#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Move.h"
#include "PrivateInclude/BoardState.h"
#include "PrivateInclude/EngineUtilities.h"

#include <algorithm>
#include <functional>
#include <assert.h>

namespace
{
	using namespace pieces;

	Square toSquare(Rank rank, File file)
	{
		return file + rank * 8;
	}

	struct SweepSearchParams
	{
		Rank rankLimit;
		File fileLimit;
		Square jump;
	};

	struct SearchResult
	{
		Piece piece;
		int32_t distance;
	};

	SearchResult sweepSearch(const BoardState& board, const SweepSearchParams& params, Square sq,
		int32_t distance)
	{
		const Piece piece = board.getPiece(sq);
		if (piece != none)
		{
			return { piece, distance };
		}

		const Rank rank = ranks::toRank(sq);
		const File file = files::toFile(sq);
		if (rank == params.rankLimit || file == params.fileLimit)
		{
			return { piece, distance };
		}

		return sweepSearch(board, params, sq + params.jump, distance + 1);
	}

	std::optional<SearchResult> sweepSearchSafe(const BoardState& board, const SweepSearchParams& params,
		Square sq, Square jump, int32_t distance)
	{
		const Rank rank = ranks::toRank(sq);
		const File file = files::toFile(sq);

		if (rank == params.rankLimit || file == params.fileLimit)
		{
			return {};
		}

		return sweepSearch(board, params, sq + params.jump, distance);
	}

	std::vector<Square> getKnightReachableSquares(Square sq)
	{
		const Rank rank = ranks::toRank(sq);
		const File file = files::toFile(sq);
		std::vector<Square> moves;
		moves.reserve(8);

		if (rank <= 5 && file <= 6)
		{
			moves.push_back(toSquare(rank + 2, file + 1));
		}
		if (rank <= 6 && file <= 5)
		{
			moves.push_back(toSquare(rank + 1, file + 2));
		}
		if (rank >= 1 && file <= 5)
		{
			moves.push_back(toSquare(rank - 1, file + 2));
		}
		if (rank >= 2 && file <= 6)
		{
			moves.push_back(toSquare(rank - 2, file + 1));
		}
		if (rank >= 2 && file >= 1)
		{
			moves.push_back(toSquare(rank - 2, file - 1));
		}
		if (rank >= 1 && file >= 2)
		{
			moves.push_back(toSquare(rank - 1, file - 2));
		}
		if (rank <= 6 && file >= 2)
		{
			moves.push_back(toSquare(rank + 1, file - 2));
		}
		if (rank <= 5 && file >= 1)
		{
			moves.push_back(toSquare(rank + 2, file - 1));
		}

		return moves;
	}

	bool isSquareInCheck(const BoardState& board, Color side, Square sq)
	{
		// The opponents pieces.
		const Piece oK = side == Color::WHITE ? bK: wK;
		const Piece oQ = side == Color::WHITE ? bQ: wQ;
		const Piece oR = side == Color::WHITE ? bR: wR;
		const Piece oB = side == Color::WHITE ? bB: wB;
		const Piece oN = side == Color::WHITE ? bN: wN;
		const Piece oP = side == Color::WHITE ? bP: wP;

		const std::function<bool(Square)> isOpponent = side == Color::WHITE ?
			EngineUtilities::isBlack : EngineUtilities::isWhite;
		const Rank kRank = ranks::toRank(sq);
		const File kFile = files::toFile(sq);

		// Check diagonals.
		{
			static constexpr SweepSearchParams upLeft{7, 0, 7};
			static constexpr SweepSearchParams upRight{7, 7, 9};
			static constexpr SweepSearchParams downRight{0, 7, -7};
			static constexpr SweepSearchParams downLeft{0, 0, -9};
			for (const auto& param: {upLeft, upRight, downRight, downLeft})
			{
				if (kRank != param.rankLimit && kFile != param.fileLimit)
				{
					if (const auto result = sweepSearchSafe(board, param, sq, param.jump, 1))
					{
						if (isOpponent(result->piece))
						{
							const bool isPawnAttack = (side == Color::WHITE && param.jump > 0) ||
								(side == Color::BLACK && param.jump < 0);
							if (result->distance == 1 &&
								((result->piece == oP && isPawnAttack) || result->piece == oK))
							{
								return true;
							}

							if (result->piece == oB || result->piece == oQ)
							{
								return true;
							}
						}
					}
				}
			}
		}

		// Check file and rank.
		{
			static constexpr SweepSearchParams up{ 7, -1, 8 };
			static constexpr SweepSearchParams down{ 0, -1, -8 };
			static constexpr SweepSearchParams left{ -1, 0, -1 };
			static constexpr SweepSearchParams right{ -1, 7, 1 };
			for (const auto& param : { up, down, left, right })
			{
				if (kRank != param.rankLimit && kFile != param.fileLimit)
				{
					if (const auto result = sweepSearchSafe(board, param, sq, param.jump, 1))
					{
						if (isOpponent(result->piece))
						{
							if (result->piece == oR || result->piece == oQ ||
								(result->piece == oK && result->distance == 1))
							{
								return true;
							}
						}
					}
				}
			}
		}

		// Check knight checks.
		for (Square nSq : getKnightReachableSquares(sq))
		{
			if (board.getPiece(nSq) == oN)
			{
				return true;
			}
		}

		return false;
	}

	bool isInCheck(const BoardState& board, Color side)
	{
		const Piece king = side == Color::WHITE ? wK : bK;
		const auto& pieces = board.getPieces();
		const auto itr = std::find(pieces.begin(), pieces.end(), king);
		assert(itr != pieces.end());
		const Square kSq = std::distance(pieces.begin(), itr);

		return isSquareInCheck(board, side, kSq);
	}

	void appendIfNotInCheck(BoardState& board, Color side, const Move& move, std::vector<Move>& moves)
	{
		// Temporarily make the move, before looking if in check.
		auto& pieces = board.getPieces();
		
		pieces[move.fromSquare] = none;
		if (move.capturedPiece != none)
		{
			pieces[move.capturedSquare] = none;
		}

		pieces[move.toSquare] = move.movingPiece;

		if (!isInCheck(board, side))
		{
			moves.push_back(move);
		}

		// Unmake the temporarily made move.
		pieces[move.toSquare] = none;
		if (move.capturedPiece != none)
		{
			pieces[move.capturedSquare] = move.capturedPiece;
		}
		
		pieces[move.fromSquare] = move.movingPiece;
	}

	void appendPawnAdvance(BoardState& board, Square sq, Color side, std::vector<Move>& moves)
	{
		const Square advance = side == Color::WHITE ? 8 : -8;
		if (board.getPiece(sq + advance) == none)
		{
			const Piece pawn = side == Color::WHITE ? wP : bP;
			appendIfNotInCheck(board, side, Move(pawn, sq, sq + advance), moves);
		}
	}

	void appendPawnDoubleAdvance(BoardState& board, Square sq, Color side, std::vector<Move>& moves)
	{
		assert((side == Color::WHITE && ranks::toRank(sq) == 1) ||
			(side == Color::BLACK && ranks::toRank(sq) == 6));

		const Square advance = side == Color::WHITE ? 16 : -16;
		assert(sq + advance >= squares::a1 && sq + advance <= squares::h8);

		if (board.getPiece(sq + advance/2) == none &&
			board.getPiece(sq + advance) == none)
		{
			const Piece pawn = side == Color::WHITE ? wP : bP;
			Move move(pawn, sq, sq + advance);
			move.enPassantCreatedSquare = sq + advance / 2;
			appendIfNotInCheck(board, side, move, moves);
		}
	}

	void appendPawnAdvanceWithPromotion(BoardState& board, Square sq, Color side,
		std::vector<Move>& moves)
	{
		assert((side == Color::WHITE && ranks::toRank(sq) == 6) ||
			(side == Color::BLACK && ranks::toRank(sq) == 1));

		const Square advance = side == Color::WHITE ? 8 : -8;
		if (board.getPiece(sq + advance) == none)
		{
			const Piece pawn = side == Color::WHITE ? wP : bP;
			const Piece Q = side == Color::WHITE ? wQ : bQ;
			const Piece R = side == Color::WHITE ? wR : bR;
			const Piece B = side == Color::WHITE ? wB : bB;
			const Piece N = side == Color::WHITE ? wN : bN;
			for (Piece promotion : {Q, R, B, N})
			{
				Move move(wP, sq, sq + advance);
				move.pawnPromotionPiece = promotion;
				appendIfNotInCheck(board, Color::WHITE, move, moves);
			}
		}
	}

	void appendPawnCapture(BoardState& board, Square sq, Color side, File file,
		std::vector<Move>& moves)
	{
		const Square offsetTowardsFirstFile = side == Color::WHITE ? 7 : -9;
		const Square offsetTowardsLastFile = side == Color::WHITE ? 9 : -7;

		for (const Square offs : {offsetTowardsFirstFile, offsetTowardsLastFile})
		{
			if ((file == 0 && offs == offsetTowardsFirstFile) ||
				(file == 7 && offs == offsetTowardsLastFile))
			{
				continue;
			}

			const auto isOpponent = side == Color::WHITE ?
				EngineUtilities::isBlack : EngineUtilities::isWhite;
			const Piece diagPiece = board.getPiece(sq + offs);
			if (isOpponent(diagPiece))
			{
				const Piece pawn = side == Color::WHITE ? wP : bP;
				appendIfNotInCheck(board, side,
					Move(pawn, sq, sq + offs, diagPiece, sq + offs), moves);
			}
		}
	}

	void appendPawnEnPassantCapture(BoardState& board, Square sq, Color side, File file,
		std::vector<Move>& moves)
	{
		assert((side == Color::WHITE && ranks::toRank(sq) == 4) ||
			(side == Color::BLACK && ranks::toRank(sq) == 3));

		const Square offsetTowardsFirstFile = side == Color::WHITE ? 7 : -9;
		const Square offsetTowardsLastFile = side == Color::WHITE ? 9 : -7;

		for (const Square offs : {offsetTowardsFirstFile, offsetTowardsLastFile})
		{
			if ((file == 0 && offs == offsetTowardsFirstFile) ||
				(file == 7 && offs == offsetTowardsLastFile))
			{
				continue;
			}

			const Square enPassSq = board.getEnPassantSquare();
			if (enPassSq != squares::none && enPassSq == sq + offs)
			{
				const Piece pawn = side == Color::WHITE ? wP : -bP;
				Move move(pawn, sq, sq + offs, board.getPiece(sq + offs - 8),
					sq + offs - 8);
				appendIfNotInCheck(board, side, move, moves);
			}
		}
	}

	void appendPawnCaptureWithPromotion(BoardState& board, Square sq, Color side, File file,
		std::vector<Move>& moves)
	{
		assert((side == Color::WHITE && ranks::toRank(sq) == 6) ||
			(side == Color::BLACK && ranks::toRank(sq) == 1));

		const auto isOpponent = side == Color::WHITE ?
			EngineUtilities::isBlack : EngineUtilities::isWhite;

		const Square offsetTowardsFirstFile = side == Color::WHITE ? 7 : -9;
		const Square offsetTowardsLastFile = side == Color::WHITE ? 9 : -7;

		for (const Square offs : {offsetTowardsFirstFile, offsetTowardsLastFile})
		{
			if ((file == 0 && offs == offsetTowardsFirstFile) ||
				(file == 7 && offs == offsetTowardsLastFile))
			{
				continue;
			}

			const Piece diagPiece = board.getPiece(sq + offs);
			if (isOpponent(diagPiece))
			{
				const Piece Q = side == Color::WHITE ? wQ : bQ;
				const Piece R = side == Color::WHITE ? wR : bR;
				const Piece B = side == Color::WHITE ? wB : bB;
				const Piece N = side == Color::WHITE ? wN : bN;
				for (Piece promotion : {Q, R, B, N})
				{
					const Piece pawn = side == Color::WHITE ? wP : bP;
					Move move(pawn, sq, sq + offs, diagPiece, sq + offs);
					move.pawnPromotionPiece = promotion;
					appendIfNotInCheck(board, side, move, moves);
				}
			}
		}
	}

	void appendWhitePawnMoves(BoardState& board, Square sq, std::vector<Move>& moves)
	{
		const Color side = Color::WHITE;
		const Rank rank = ranks::toRank(sq);

		switch (rank)
		{
			case 1:
				appendPawnAdvance(board, sq, side, moves);
				appendPawnDoubleAdvance(board, sq, side, moves);
				appendPawnCapture(board, sq, side, files::toFile(sq), moves);
				break;
			case 4:
			{
				const File file = files::toFile(sq);
				appendPawnAdvance(board, sq, side, moves);
				appendPawnCapture(board, sq, side, file, moves);
				appendPawnEnPassantCapture(board, sq, side, file, moves);
				break;
			}
			case 6:
				appendPawnAdvanceWithPromotion(board, sq, side, moves);
				appendPawnCaptureWithPromotion(board, sq, side, files::toFile(sq), moves);
				break;
			default:
				appendPawnAdvance(board, sq, side, moves);
				appendPawnCapture(board, sq, side, files::toFile(sq), moves);
		}
	}

	void appendBlackPawnMoves(BoardState& board, Square sq, std::vector<Move>& moves)
	{
		const Color side = Color::BLACK;
		const Rank rank = ranks::toRank(sq);

		switch (rank)
		{
			case 1:
				appendPawnAdvanceWithPromotion(board, sq, side, moves);
				appendPawnCaptureWithPromotion(board, sq, side, files::toFile(sq), moves);
				break;
			case 3:
			{
				const File file = files::toFile(sq);
				appendPawnAdvance(board, sq, side, moves);
				appendPawnCapture(board, sq, side, file, moves);
				appendPawnEnPassantCapture(board, sq, side, file, moves);
				break;
			}
			case 6:
				appendPawnAdvance(board, sq, side, moves);
				appendPawnDoubleAdvance(board, sq, side, moves);
				appendPawnCapture(board, sq, side, files::toFile(sq), moves);
				break;
			default:
				appendPawnAdvance(board, sq, side, moves);
				appendPawnCapture(board, sq, side, files::toFile(sq), moves);
		}
	}

	void appendKnightMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		const Piece knight = side == Color::WHITE ? wN : bN;
		const auto& isOpponent = side == Color::WHITE ? 
			EngineUtilities::isBlack : EngineUtilities::isWhite;

		for (Square nSq : getKnightReachableSquares(sq))
		{
			const Piece other = board.getPiece(nSq);
			if (other == none)
			{
				appendIfNotInCheck(board, side, Move(knight, sq, nSq), moves);
			}
			else if (isOpponent(other))
			{
				appendIfNotInCheck(board, side, Move(knight, sq, nSq, other, nSq), moves);
			}
		}
	}

	std::vector<Move> getDiagonalMoves(BoardState& board, Color side, Square sq)
	{
		static constexpr SweepSearchParams parUl{7, 0, 7}; // Up left.
		static constexpr SweepSearchParams parUr{7, 7, 9}; // Up right.
		static constexpr SweepSearchParams parDr{0, 7, -7}; // Down right.
		static constexpr SweepSearchParams parDl{0, 0, -9}; // Down left.
		const auto& isOpponent = side == Color::WHITE ?
			EngineUtilities::isBlack : EngineUtilities::isWhite;

		static constexpr size_t maxMoves = 7;
		std::vector<Move> moves;
		moves.reserve(maxMoves);
		for (const auto& params : {parUl, parUr, parDr, parDl})
		{
			if (const auto sr = sweepSearchSafe(board, params, sq, params.jump, 1))
			{
				for (int32_t i = 1; i <= sr->distance; i++)
				{
					const Square diagSq = sq + params.jump * i;
					const Piece diagPiece = board.getPiece(diagSq);
					if (diagPiece == none)
					{
						moves.push_back(Move(board.getPiece(sq), sq, diagSq));
					}
					else if (isOpponent(diagPiece))
					{
						moves.push_back(Move(board.getPiece(sq), sq, diagSq, diagPiece, diagSq));
					}
					else
					{
						// Encountered same colored piece.
						break;
					}
				}
			}
		}

		return moves;
	}

	void appendBishopMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		for (const auto& move : getDiagonalMoves(board, side, sq))
		{
			appendIfNotInCheck(board, side, move, moves);
		}
	}

	std::vector<Move> getStraightMoves(BoardState& board, Color side, Square sq)
	{
		static constexpr SweepSearchParams parUl{ 7, -1, 8 }; // Up.
		static constexpr SweepSearchParams parUr{ -1, 7, 1 }; // Right.
		static constexpr SweepSearchParams parDr{ 0, -1, -8}; // Down.
		static constexpr SweepSearchParams parDl{ -1, 0, -1 }; // Left.
		const auto& isOpponent = side == Color::WHITE ?
			EngineUtilities::isBlack : EngineUtilities::isWhite;
		static constexpr size_t maxMoves = 7;
		std::vector<Move> moves;
		moves.reserve(maxMoves);

		for (const auto& params : { parUl, parUr, parDr, parDl })
		{
			if (const auto sr = sweepSearchSafe(board, params, sq, params.jump, 1))
			{
				for (int32_t i = 1; i <= sr->distance; i++)
				{
					const Square diagSq = sq + params.jump * i;
					const Piece diagPiece = board.getPiece(diagSq);
					if (diagPiece == none)
					{
						moves.push_back(Move(board.getPiece(sq), sq, diagSq));
					}
					else if (isOpponent(diagPiece))
					{
						moves.push_back(Move(board.getPiece(sq), sq, diagSq, diagPiece, diagSq));
					}
					else
					{
						// Encountered same colored piece.
						break;
					}
				}
			}
		}

		return moves;
	}

	void signalMayProhibitCasteling(BoardState& board, Color side,
		bool mayProhibitKingSide, bool mayProhibitQueenSide, Move& move)
	{
		auto& castlingAvailability = board.getCastleAvailability();
		if (side == Color::WHITE)
		{
			if (mayProhibitKingSide && castlingAvailability['K'])
			{
				move.prohibitsWKcastling = true;
			}
			if (mayProhibitQueenSide && castlingAvailability['Q'])
			{
				move.prohibitsWQcastling = true;
			}
		}
		else
		{
			if (mayProhibitKingSide && castlingAvailability['k'])
			{
				move.prohibitsBKcastling = true;
			}
			if (mayProhibitQueenSide && castlingAvailability['q'])
			{
				move.prohibitsBQcastling = true;
			}
		}
	}

	void appendRookMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		const File file = files::toFile(sq);
		for (auto& move : getStraightMoves(board, side, sq))
		{
			switch (file)
			{
				case 0:
					signalMayProhibitCasteling(board, side, false, true, move);
					break;
				case 7:
					signalMayProhibitCasteling(board, side, true, false, move);
					break;
			}

			appendIfNotInCheck(board, side, move, moves);
		}
	}

	void appendQueenMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		for (const auto& move : getDiagonalMoves(board, side, sq))
		{
			appendIfNotInCheck(board, side, move, moves);
		}

		for (const auto& move : getStraightMoves(board, side, sq))
		{
			appendIfNotInCheck(board, side, move, moves);
		}
	}

	void appendKingMove(BoardState& board, Color side, Square sq,
		std::vector<Move>& moves, Square offs)
	{
		assert(sq + offs <= squares::h8 && sq + offs >= squares::a1);
		const auto& isOpponent = side == Color::WHITE ?
			EngineUtilities::isBlack : EngineUtilities::isWhite;
		const Piece other = board.getPiece(sq + offs);

		if (other == none)
		{
			Move move(board.getPiece(sq), sq, sq + offs);
			signalMayProhibitCasteling(board, side, true, true, move);
			appendIfNotInCheck(board, side, move, moves);
		}
		else if (isOpponent(other))
		{
			Move move(board.getPiece(sq), sq, sq + offs, other, sq + offs);
			signalMayProhibitCasteling(board, side, true, true, move);
			appendIfNotInCheck(board, side, move, moves);
		}
	}

	void appendKingCastlingMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		auto& castlingAvailability = board.getCastleAvailability();
		const auto& pieces = board.getPieces();

		if (side == Color::WHITE)
		{
			if (castlingAvailability['K'] && pieces[squares::f1] == none && pieces[squares::g1] == none &&
				!isSquareInCheck(board, side, squares::f1))
			{
				Move move(pieces[sq], squares::e1, squares::g1);
				signalMayProhibitCasteling(board, side, true, true, move);
				appendIfNotInCheck(board, side, move, moves);
			}
			if (castlingAvailability['Q'] && pieces[squares::b1] == none && pieces[squares::c1] == none &&
				pieces[squares::d1] == none && !isSquareInCheck(board, side, squares::d1))
			{
				Move move(pieces[sq], squares::e1, squares::c1);
				signalMayProhibitCasteling(board, side, true, true, move);
				appendIfNotInCheck(board, side, move, moves);
			}
		}
		else
		{
			if (castlingAvailability['k'] && pieces[squares::f8] == none && pieces[squares::g8] == none &&
				!isSquareInCheck(board, side, squares::f8))
			{
				Move move(pieces[sq], squares::e8, squares::g8);
				signalMayProhibitCasteling(board, side, true, true, move);
				appendIfNotInCheck(board, side, move, moves);
			}
			if (castlingAvailability['q'] && pieces[squares::b8] == none && pieces[squares::c8] == none &&
				pieces[squares::d8] == none && !isSquareInCheck(board, side, squares::d8))
			{
				Move move(pieces[sq], squares::e8, squares::c8);
				signalMayProhibitCasteling(board, side, true, true, move);
				appendIfNotInCheck(board, side, move, moves);
			}
		}
	}

	void appendKingMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		const Rank rank = ranks::toRank(sq);
		const File file = files::toFile(sq);		

		if (rank >= 1)
		{
			appendKingMove(board, side, sq, moves, -8);
			if (file >= 1)
			{
				appendKingMove(board, side, sq, moves, -9);
			}
			if (file <= 6)
			{
				appendKingMove(board, side, sq, moves, -7);
			}
		}


		if (rank <= 6)
		{
			appendKingMove(board, side, sq, moves, 8);
			if (file >= 1)
			{
				appendKingMove(board, side, sq, moves, 7);
			}
			if (file <= 6)
			{
				appendKingMove(board, side, sq, moves, 9);
			}
		}

		if (file >= 1)
		{
			appendKingMove(board, side, sq, moves, -1);
		}

		if (file <= 6)
		{
			appendKingMove(board, side, sq, moves, 1);
		}

		appendKingCastlingMoves(board, side, sq, moves);
	}

	std::vector<Move> getLegalWhiteMoves(BoardState& board)
	{
		static constexpr size_t probableMax = 100;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			switch (board.getPiece(sq))
			{ 
				case wP:
					appendWhitePawnMoves(board, sq, moves);
					break;
				case wN:
					appendKnightMoves(board, Color::WHITE, sq, moves);
					break;
				case wB:
					appendBishopMoves(board, Color::WHITE, sq, moves);
					break;
				case wR:
					appendRookMoves(board, Color::WHITE, sq, moves);
					break;
				case wQ:
					appendQueenMoves(board, Color::WHITE, sq, moves);
					break;
				case wK:
					appendKingMoves(board, Color::WHITE, sq, moves);
					break;
			}
		}

		return moves;
	}

	std::vector<Move> getLegalBlackMoves(BoardState& board)
	{
		static constexpr size_t probableMax = 100;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			switch (board.getPiece(sq))
			{
			case bP:
				appendBlackPawnMoves(board, sq, moves);
				break;
			case bN:
				appendKnightMoves(board, Color::BLACK, sq, moves);
				break;
			case bB:
				appendBishopMoves(board, Color::BLACK, sq, moves);
				break;
			case bR:
				appendRookMoves(board, Color::BLACK, sq, moves);
				break;
			case bQ:
				appendQueenMoves(board, Color::BLACK, sq, moves);
				break;
			case bK:
				appendKingMoves(board, Color::BLACK, sq, moves);
				break;
			}
		}

		return moves;
	}
}

std::vector<Move> Engine::getLegalMoves(BoardState& board) const
{
	if (board.getTurn() == pieces::Color::WHITE)
	{
		return getLegalWhiteMoves(board);
	}
	else
	{
		return getLegalBlackMoves(board);
	}
}
