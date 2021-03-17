#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Move.h"
#include "PrivateInclude/BoardState.h"
#include "PrivateInclude/EngineUtilities.h"

#include <algorithm>
#include <functional>
#include <cassert>

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

	void signalMayProhibitCastelingKingOrRookMove(const BoardState& board, Color side,
		bool mayProhibitKingSide, bool mayProhibitQueenSide, Move& move)
	{
		auto& castlingAvailability = board.getCastleAvailability();
		if (side == Color::WHITE)
		{
			assert(board.getPiece(move.fromSquare) == wK || board.getPiece(move.fromSquare) == wR);
			if (mayProhibitKingSide && castlingAvailability.find('K')->second)
			{
				move.prohibitsWKcastling = true;
			}
			if (mayProhibitQueenSide && castlingAvailability.find('Q')->second)
			{
				move.prohibitsWQcastling = true;
			}
		}
		else
		{
			assert(board.getPiece(move.fromSquare) == bK || board.getPiece(move.fromSquare) == bR);
			if (mayProhibitKingSide && castlingAvailability.find('k')->second)
			{
				move.prohibitsBKcastling = true;
			}
			if (mayProhibitQueenSide && castlingAvailability.find('q')->second)
			{
				move.prohibitsBQcastling = true;
			}
		}
	}

	void signalMayProhibitCastelingRookCaptured(const BoardState& board, Color side,
		bool mayProhibitKingSide, bool mayProhibitQueenSide, Move& move)
	{
		auto& castlingAvailability = board.getCastleAvailability();
		if (side == Color::WHITE)
		{
			assert(move.capturedPiece == bR);
			if (mayProhibitKingSide && castlingAvailability.find('k')->second)
			{
				move.prohibitsBKcastling = true;
			}
			if (mayProhibitQueenSide && castlingAvailability.find('q')->second)
			{
				move.prohibitsBQcastling = true;
			}
		}
		else
		{
			assert(move.capturedPiece == wR);
			if (mayProhibitKingSide && castlingAvailability.find('K')->second)
			{
				move.prohibitsWKcastling = true;
			}
			if (mayProhibitQueenSide && castlingAvailability.find('Q')->second)
			{
				move.prohibitsWQcastling = true;
			}
		}
	}

	// In case a rook is captured, it may prohibit casteling rights. This must be checked explicitly
	// since in theory the non-captured rook could take its place, and then casteling is not legal.
	void handleRookCaptureCastleAvailability(const BoardState& board, Color side, Move& move)
	{
		if (side == Color::WHITE)
		{
			if (move.capturedPiece != bR)
			{
				return;
			}

			if (move.capturedSquare == squares::a8)
			{
				signalMayProhibitCastelingRookCaptured(board, side, false, true, move);
			}
			else if (move.capturedSquare == squares::h8)
			{
				signalMayProhibitCastelingRookCaptured(board, side, true, false, move);
			}
		}
		else
		{
			if (move.capturedPiece != wR)
			{
				return;
			}

			if (move.capturedSquare == squares::a1)
			{
				signalMayProhibitCastelingRookCaptured(board, side, false, true, move);
			}
			else if (move.capturedSquare == squares::h1)
			{
				signalMayProhibitCastelingRookCaptured(board, side, true, false, move);
			}
		}
	}

	bool isCastelingMove(const Move& move)
	{
		if (move.movingPiece == wK && move.fromSquare == squares::e1 &&
			(move.toSquare == squares::g1 || move.toSquare == squares::c1))
		{
			return true;
		}

		if (move.movingPiece == bK && move.fromSquare == squares::e8 &&
			(move.toSquare == squares::g8 || move.toSquare == squares::c8))
		{
			return true;
		}

		return false;
	}

	void doRookMoveForTemporaryCastling(BoardState& board, const Move& move)
	{
		assert(move.movingPiece == wK || move.movingPiece == bK);
		assert(move.fromSquare == squares::e1 || move.fromSquare == squares::e8);
		assert(isCastelingMove(move));

		if (move.movingPiece == wK)
		{
			if (move.toSquare == squares::g1)
			{
				board.getPieces()[squares::h1] = none;
				board.getPieces()[squares::f1] = wR;
			}
			else if (move.toSquare == squares::c1)
			{
				board.getPieces()[squares::a1] = none;
				board.getPieces()[squares::d1] = wR;
			}
		}
		else
		{
			if (move.toSquare == squares::g8)
			{
				board.getPieces()[squares::h8] = none;
				board.getPieces()[squares::f8] = bR;
			}
			else if (move.toSquare == squares::c8)
			{
				board.getPieces()[squares::a8] = none;
				board.getPieces()[squares::d8] = bR;
			}
		}
	}

	void undoRookMoveForTemporaryCastling(BoardState& board, const Move& move)
	{
		assert(move.movingPiece == wK || move.movingPiece == bK);
		assert(move.fromSquare == squares::e1 || move.fromSquare == squares::e8);
		assert(isCastelingMove(move));

		if (move.movingPiece == wK)
		{
			if (move.toSquare == squares::g1)
			{
				board.getPieces()[squares::h1] = wR;
				board.getPieces()[squares::f1] = none;
			}
			else if (move.toSquare == squares::c1)
			{
				board.getPieces()[squares::a1] = wR;
				board.getPieces()[squares::d1] = none;
			}
		}
		else
		{
			if (move.toSquare == squares::g8)
			{
				board.getPieces()[squares::h8] = bR;
				board.getPieces()[squares::f8] = none;
			}
			else if (move.toSquare == squares::c8)
			{
				board.getPieces()[squares::a8] = bR;
				board.getPieces()[squares::d8] = none;
			}
		}
	}

	void appendIfNotInCheck(BoardState& board, Color side, Move& move, std::vector<Move>& moves)
	{
#ifndef NDEBUG
		const BoardState boardPreMove = board;
#endif

		// Temporarily make the move, before looking if in check.
		auto& pieces = board.getPieces();
		
		pieces[move.fromSquare] = none;
		if (move.capturedPiece != none)
		{
			pieces[move.capturedSquare] = none;
		}

		pieces[move.toSquare] = move.movingPiece;

		// In case this is a castling move, we have to take care of the rook.
		const bool isCasteling = isCastelingMove(move);
		if (isCasteling)
		{
			doRookMoveForTemporaryCastling(board, move);
		}

		if (!isInCheck(board, side))
		{
			handleRookCaptureCastleAvailability(board, side, move);

			if (isCasteling && side == Color::WHITE)
			{
				if (move.toSquare == squares::g1)
				{
					assert(board.getPiece(squares::h1) == none && board.getPiece(squares::g1) == wK &&
						board.getPiece(squares::f1) == wR && board.getPiece(squares::e1) == none && board.getCastleAvailability()['K']);
				}				
				else if (move.toSquare == squares::c1)
				{
					assert(board.getPiece(squares::a1) == none && board.getPiece(squares::c1) == wK &&
						board.getPiece(squares::d1) == wR && board.getPiece(squares::e1) == none && board.getPiece(squares::b1) == none
						&& board.getCastleAvailability()['Q']);
				}						
				else
				{
					assert(false);
				}
			}
			if (isCasteling && side == Color::BLACK)
			{
				if (move.toSquare == squares::g8)
				{
					assert(board.getPiece(squares::h8) == none && board.getPiece(squares::g8) == bK &&
						board.getPiece(squares::f8) == bR && board.getPiece(squares::e8) == none && board.getCastleAvailability()['k']);
				}
				else if (move.toSquare == squares::c8)
				{
					assert(board.getPiece(squares::a8) == none && board.getPiece(squares::c8) == bK &&
						board.getPiece(squares::d8) == bR && board.getPiece(squares::e8) == none && board.getPiece(squares::b8) == none
						&& board.getCastleAvailability()['q']);
				}
				else
				{
					assert(false);
				}
			}

			moves.push_back(move);
		}

		// Unmake the temporarily made move.
		pieces[move.toSquare] = none;
		if (move.capturedPiece != none)
		{
			pieces[move.capturedSquare] = move.capturedPiece;
		}
		
		pieces[move.fromSquare] = move.movingPiece;

		// Take care of the rook if this was a casteling move.
		if (isCasteling)
		{
			undoRookMoveForTemporaryCastling(board, move);
		}

#ifndef NDEBUG
		assert(boardPreMove.getPieces() == board.getPieces());
		assert(boardPreMove.getCastleAvailability() == board.getCastleAvailability());
		assert(boardPreMove.getEnPassantSquare() == board.getEnPassantSquare());
		assert(boardPreMove.getTurn() == board.getTurn());
#endif
	}

	void appendPawnAdvance(BoardState& board, Square sq, Color side, std::vector<Move>& moves)
	{
		const Square advance = side == Color::WHITE ? 8 : -8;
		if (board.getPiece(sq + advance) == none)
		{
			appendIfNotInCheck(board, side,
				Move(board.getPiece(sq), sq, sq + advance, board.getEnPassantSquare()), moves);
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
			Move move(board.getPiece(sq), sq, sq + advance, board.getEnPassantSquare());
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
			const Piece Q = side == Color::WHITE ? wQ : bQ;
			const Piece R = side == Color::WHITE ? wR : bR;
			const Piece B = side == Color::WHITE ? wB : bB;
			const Piece N = side == Color::WHITE ? wN : bN;
			for (Piece promotion : {Q, R, B, N})
			{
				Move move(board.getPiece(sq), sq, sq + advance, board.getEnPassantSquare());
				move.pawnPromotionPiece = promotion;
				appendIfNotInCheck(board, side, move, moves);
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
				appendIfNotInCheck(board, side, Move(board.getPiece(sq), sq, sq + offs, board.getEnPassantSquare(),
					diagPiece, sq + offs),moves);
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
				const Square enPassEnemyOffs = side == Color::WHITE ? -8 : 8;
				Move move(board.getPiece(sq), sq, sq + offs, enPassSq,
					board.getPiece(enPassSq + enPassEnemyOffs), enPassSq + enPassEnemyOffs);
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
					Move move(board.getPiece(sq), sq, sq + offs, board.getEnPassantSquare(), diagPiece, sq + offs);
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
				appendIfNotInCheck(board, side, Move(knight, sq, nSq, board.getEnPassantSquare()), moves);
			}
			else if (isOpponent(other))
			{
				appendIfNotInCheck(board, side,
					Move(knight, sq, nSq, board.getEnPassantSquare(), other, nSq), moves);
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
						moves.push_back(Move(board.getPiece(sq), sq, diagSq, board.getEnPassantSquare()));
					}
					else if (isOpponent(diagPiece))
					{
						moves.push_back(Move(
							board.getPiece(sq), sq, diagSq, board.getEnPassantSquare(), diagPiece, diagSq));
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

	bool fast_rookCaptureProhibitsWQcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (captureSquare == squares::a1 && board.getCastleAvailability().find('Q')->second)
		{
			return true;
		}

		return false;
	}

	bool fast_rookCaptureProhibitsWKcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (captureSquare == squares::h1 && board.getCastleAvailability().find('K')->second)
		{
			return true;
		}

		return false;
	}

	bool fast_rookCaptureProhibitsBQcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (captureSquare == squares::a8 && board.getCastleAvailability().find('q')->second)
		{
			return true;
		}

		return false;
	}

	bool fast_rookCaptureProhibitsBKcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (captureSquare == squares::h8 && board.getCastleAvailability().find('k')->second)
		{
			return true;
		}

		return false;
	}


	// Note: Does not handle en passant square creation, pawn promotion, casteling or castling avaialibity.
	Move fast_createRegularSilentMove(const BoardState& board, Square fromSquare, Square toSquare)
	{
		return Move(board.getPiece(fromSquare), fromSquare, toSquare, board.getEnPassantSquare());
	}

	// Note: Does not handle pawn promotion, en passant captures or castling avaialibity
	// due to rook or king moved. Does handle castling availability due to captured rook though.
	Move fast_createWhiteRegularCapturingMove(const BoardState& board, Square fromSquare, Square toSquare)
	{
		assert(board.getTurn() == pieces::Color::WHITE);

		const Piece capturedPiece = board.getPiece(toSquare);
		Move move(board.getPiece(fromSquare), fromSquare, toSquare, board.getEnPassantSquare(),
			capturedPiece, toSquare);

		// Handle castling availability if a rook was captured.
		if (capturedPiece == pieces::bR)
		{
			if (fast_rookCaptureProhibitsBKcastling(board, toSquare))
			{
				move.prohibitsBKcastling = true;
			}
			else if (fast_rookCaptureProhibitsBQcastling(board, toSquare))
			{
				move.prohibitsBQcastling = true;
			}
		}

		return move;
	}

	// Note: Does not handle pawn promotion, en passant captures or castling avaialibity
	// due to rook or king moved. Does handle castling availability due to captured rook though.
	Move fast_createBlackRegularCapturingMove(const BoardState& board, Square fromSquare, Square toSquare)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		const Piece capturedPiece = board.getPiece(toSquare);
		Move move(board.getPiece(fromSquare), fromSquare, toSquare, board.getEnPassantSquare(),
			capturedPiece, toSquare);

		// Handle castling availability if a rook was captured.
		if (capturedPiece == pieces::wR)
		{
			if (fast_rookCaptureProhibitsWKcastling(board, toSquare))
			{
				move.prohibitsWKcastling = true;
			}
			else if (fast_rookCaptureProhibitsWQcastling(board, toSquare))
			{
				move.prohibitsWQcastling = true;
			}
		}

		return move;
	}

	void fast_appendWhiteRegularSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(EngineUtilities::isWhite(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				appendIfNotInCheck(board, Color::WHITE,
					fast_createRegularSilentMove(board, sq, nextSq), moves);
			}
			else if (EngineUtilities::isBlack(other))
			{
				appendIfNotInCheck(board, Color::WHITE,
					fast_createWhiteRegularCapturingMove(board, sq, nextSq), moves);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendBlackRegularSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(EngineUtilities::isBlack(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				appendIfNotInCheck(board, Color::BLACK, fast_createRegularSilentMove(board, sq, nextSq), moves);
			}
			else if (EngineUtilities::isWhite(other))
			{
				appendIfNotInCheck(board, Color::BLACK,
					fast_createBlackRegularCapturingMove(board, sq, nextSq), moves);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendWhiteRegularNonSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(EngineUtilities::isWhite(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				appendIfNotInCheck(board, Color::WHITE,
					fast_createRegularSilentMove(board, sq, nextSq), moves);
			}
			else if (EngineUtilities::isBlack(other))
			{
				appendIfNotInCheck(board, Color::WHITE,
					fast_createWhiteRegularCapturingMove(board, sq, nextSq), moves);
			}
		}
	}

	void fast_appendBlackRegularNonSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(EngineUtilities::isBlack(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				appendIfNotInCheck(board, Color::BLACK, fast_createRegularSilentMove(board, sq, nextSq), moves);
			}
			else if (EngineUtilities::isWhite(other))
			{
				appendIfNotInCheck(board, Color::BLACK,
					fast_createBlackRegularCapturingMove(board, sq, nextSq), moves);
			}
		}
	}

	bool fast_rookMoveProhibitsWKCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (fromSquare == squares::h1 && board.getCastleAvailability().find('K')->second)
		{
			return true;
		}

		return false;
	}

	bool fast_rookMoveProhibitsWQCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (fromSquare == squares::a1 && board.getCastleAvailability().find('Q')->second)
		{
			return true;
		}

		return false;
	}

	bool fast_rookMoveProhibitsBKCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (fromSquare == squares::h8 && board.getCastleAvailability().find('k')->second)
		{
			return true;
		}

		return false;
	}

	bool fast_rookMoveProhibitsBQCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (fromSquare == squares::a8 && board.getCastleAvailability().find('q')->second)
		{
			return true;
		}

		return false;
	}

	void fast_appendWhiteRookSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(board.getPiece(sq) == pieces::wR);
		const bool prohibitsWKcastle = fast_rookMoveProhibitsWKCastling(board, sq);
		const bool prohibitsWQcastle = fast_rookMoveProhibitsWQCastling(board, sq);
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, nextSq);
				move.prohibitsWKcastling = prohibitsWKcastle;
				move.prohibitsWQcastling = prohibitsWQcastle;
				appendIfNotInCheck(board, Color::WHITE,	move, moves);
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = fast_createWhiteRegularCapturingMove(board, sq, nextSq);
				move.prohibitsWKcastling = prohibitsWKcastle;
				move.prohibitsWQcastling = prohibitsWQcastle;
				appendIfNotInCheck(board, Color::WHITE, move, moves);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendBlackRookSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(board.getPiece(sq) == pieces::bR);
		const bool prohibitsBKcastle = fast_rookMoveProhibitsBKCastling(board, sq);
		const bool prohibitsBQcastle = fast_rookMoveProhibitsBQCastling(board, sq);
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, nextSq);
				move.prohibitsBKcastling = prohibitsBKcastle;
				move.prohibitsBQcastling = prohibitsBQcastle;
				appendIfNotInCheck(board, Color::BLACK, move, moves);
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = fast_createBlackRegularCapturingMove(board, sq, nextSq);
				move.prohibitsBKcastling = prohibitsBKcastle;
				move.prohibitsBQcastling = prohibitsBQcastle;
				appendIfNotInCheck(board, Color::BLACK, move, moves);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendWhitePawnAdvanceWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);

		for (const auto promotion : {pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = fast_createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			appendIfNotInCheck(board, pieces::Color::WHITE, move, moves);
		}
	}

	void fast_appendBlackPawnAdvanceWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = fast_createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			appendIfNotInCheck(board, pieces::Color::BLACK, move, moves);
		}
	}

	void fast_appendWhitePawnCaptureWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);
		assert(EngineUtilities::isBlack(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = fast_createWhiteRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			appendIfNotInCheck(board, pieces::Color::WHITE, move, moves);
		}
	}

	void fast_appendBlackPawnCaptureWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);
		assert(EngineUtilities::isWhite(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = fast_createBlackRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			appendIfNotInCheck(board, pieces::Color::BLACK, move, moves);
		}
	}

	void fast_appendWhiteKingMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(board.getPiece(sq) == pieces::wK);

		const bool castleKAvailable = board.getCastleAvailability().find('K')->second;
		const bool castleQAvailable = board.getCastleAvailability().find('Q')->second;

		// Non castling moves.
		for (const Square toSq : fastSqLookup.getkingNonCastlingReachableSquares()[sq])
		{
			const Piece other = board.getPiece(toSq);
			if (other == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, toSq);
				move.prohibitsWKcastling = castleKAvailable;
				move.prohibitsWQcastling = castleQAvailable;
				appendIfNotInCheck(board, Color::WHITE, move, moves);
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = fast_createWhiteRegularCapturingMove(board, sq, toSq);
				move.prohibitsWKcastling = castleKAvailable;
				move.prohibitsWQcastling = castleQAvailable;
				appendIfNotInCheck(board, Color::WHITE, move, moves);
			}
		}

		// Castling moves.
		if (sq == squares::e1 && board.getPiece(squares::f1) == pieces::none &&
			board.getPiece(squares::g1) == pieces::none && castleKAvailable &&
			!isSquareInCheck(board, pieces::Color::WHITE, squares::f1))
		{
			assert(board.getPiece(squares::h1) == pieces::wR);
			Move move = fast_createRegularSilentMove(board, sq, squares::g1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			appendIfNotInCheck(board, Color::WHITE, move, moves);
		}

		if (sq == squares::e1 && board.getPiece(squares::b1) == pieces::none &&
			board.getPiece(squares::c1) == pieces::none && board.getPiece(squares::d1) == pieces::none
			&& castleQAvailable && !isSquareInCheck(board, pieces::Color::WHITE, squares::d1))
		{
			assert(board.getPiece(squares::a1) == pieces::wR);
			Move move = fast_createRegularSilentMove(board, sq, squares::c1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			appendIfNotInCheck(board, Color::WHITE, move, moves);
		}
	}

	void fast_appendBlackKingMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(board.getPiece(sq) == pieces::bK);

		const bool castleKAvailable = board.getCastleAvailability().find('k')->second;
		const bool castleQAvailable = board.getCastleAvailability().find('q')->second;

		// Non castling moves.
		for (const Square toSq : fastSqLookup.getkingNonCastlingReachableSquares()[sq])
		{
			const Piece other = board.getPiece(toSq);
			if (other == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, toSq);
				move.prohibitsBKcastling = castleKAvailable;
				move.prohibitsBQcastling = castleQAvailable;
				appendIfNotInCheck(board, Color::BLACK, move, moves);
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = fast_createBlackRegularCapturingMove(board, sq, toSq);
				move.prohibitsBKcastling = castleKAvailable;
				move.prohibitsBQcastling = castleQAvailable;
				appendIfNotInCheck(board, Color::BLACK, move, moves);
			}
		}

		// Castling moves.
		if (sq == squares::e8 && board.getPiece(squares::f8) == pieces::none &&
			board.getPiece(squares::g8) == pieces::none && castleKAvailable &&
			!isSquareInCheck(board, pieces::Color::BLACK, squares::f8))
		{
			assert(board.getPiece(squares::h8) == pieces::bR);
			Move move = fast_createRegularSilentMove(board, sq, squares::g8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			appendIfNotInCheck(board, Color::BLACK, move, moves);
		}

		if (sq == squares::e8 && board.getPiece(squares::b8) == pieces::none &&
			board.getPiece(squares::c8) == pieces::none && board.getPiece(squares::d8) == pieces::none
			&& castleQAvailable && !isSquareInCheck(board, pieces::Color::BLACK, squares::d8))
		{
			assert(board.getPiece(squares::a8) == pieces::bR);
			Move move = fast_createRegularSilentMove(board, sq, squares::c8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			appendIfNotInCheck(board, Color::BLACK, move, moves);
		}
	}

	void fast_appendWhitePawnMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(board.getPiece(sq) == pieces::wP);
		assert(ranks::toRank(sq) <= 6); // A pawn should never be at the last rank, ready to move.

		const Rank rank = ranks::toRank(sq);

		// Single pawn advance.
		const Square singleAdvanceSq = sq + 8;
		if (board.getPiece(singleAdvanceSq) == pieces::none)
		{
			if (rank == 6) // Pawn promotion.
			{
				fast_appendWhitePawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves);
			}
			else
			{
				appendIfNotInCheck(board, pieces::Color::WHITE, fast_createRegularSilentMove(
					board, sq, singleAdvanceSq), moves);
			}
				

			// Double pawn advance.
			const Square doubleAdvanceSq = sq + 16;
			if (rank == 1 && board.getPiece(doubleAdvanceSq) == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				appendIfNotInCheck(board, pieces::Color::WHITE, move, moves);
			}
		}

		// Pawn captures.
		for (const Square captureSq : fastSqLookup.getWhitePawnCaptureSquares()[sq])
		{
			const Piece other = board.getPiece(captureSq);
			if (EngineUtilities::isBlack(other))
			{
				if (rank == 6)
				{
					fast_appendWhitePawnCaptureWithPromotions(board, sq, captureSq, moves);
				}
				else
				{
					appendIfNotInCheck(board, pieces::Color::WHITE,
						fast_createWhiteRegularCapturingMove(board, sq, captureSq), moves);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq - 8) == pieces::bP);
				appendIfNotInCheck(board, pieces::Color::WHITE,
					Move(pieces::wP, sq, captureSq, captureSq, pieces::bP, captureSq - 8), moves);
			}
		}
	}

	void fast_appendBlackPawnMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(board.getPiece(sq) == pieces::bP);
		assert(ranks::toRank(sq) >= 1); // A pawn should never be at the last rank, ready to move.

		const Rank rank = ranks::toRank(sq);

		// Single pawn advance.
		const Square singleAdvanceSq = sq - 8;
		if (board.getPiece(singleAdvanceSq) == pieces::none)
		{
			if (rank == 1) // Pawn promotion.
			{
				fast_appendBlackPawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves);
			}
			else
			{
				appendIfNotInCheck(board, pieces::Color::BLACK, fast_createRegularSilentMove(
					board, sq, singleAdvanceSq), moves);
			}


			// Double pawn advance.
			const Square doubleAdvanceSq = sq - 16;
			if (rank == 6 && board.getPiece(doubleAdvanceSq) == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				appendIfNotInCheck(board, pieces::Color::BLACK, move, moves);
			}
		}

		// Pawn captures.
		for (const Square captureSq : fastSqLookup.getBlackPawnCaptureSquares()[sq])
		{
			const Piece other = board.getPiece(captureSq);
			if (EngineUtilities::isWhite(other))
			{
				if (rank == 1)
				{
					fast_appendBlackPawnCaptureWithPromotions(board, sq, captureSq, moves);
				}
				else
				{
					appendIfNotInCheck(board, pieces::Color::BLACK,
						fast_createBlackRegularCapturingMove(board, sq, captureSq), moves);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq + 8) == pieces::wP);
				appendIfNotInCheck(board, pieces::Color::BLACK,
					Move(pieces::bP, sq, captureSq, captureSq, pieces::wP, captureSq + 8), moves);
			}
		}
	}

	void fast_appendWhiteBishopMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves);
	}

	void fast_appendBlackBishopMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves);
	}

	void fast_appendWhiteKnightMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRegularNonSweepMoves(board, sq, fastSqLookup.getknightReachableSquares(), moves);
	}

	void fast_appendBlackKnightMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRegularNonSweepMoves(board, sq, fastSqLookup.getknightReachableSquares(), moves);
	}

	void fast_appendWhiteRookMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves);
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves);
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves);
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves);
	}

	void fast_appendBlackRookMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves);
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves);
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves);
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves);
	}

	void fast_appendWhiteQueenMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves);
	}

	void fast_appendBlackQueenMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves);
	}

	void appendBishopMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		for (auto& move : getDiagonalMoves(board, side, sq))
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
						moves.push_back(Move(board.getPiece(sq), sq, diagSq, board.getEnPassantSquare()));
					}
					else if (isOpponent(diagPiece))
					{
						moves.push_back(Move(
							board.getPiece(sq), sq, diagSq, board.getEnPassantSquare(), diagPiece, diagSq));
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

	void appendRookMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		const File file = files::toFile(sq);
		for (auto& move : getStraightMoves(board, side, sq))
		{
			switch (file)
			{
				case 0:
					signalMayProhibitCastelingKingOrRookMove(board, side, false, true, move);
					break;
				case 7:
					signalMayProhibitCastelingKingOrRookMove(board, side, true, false, move);
					break;
			}

			appendIfNotInCheck(board, side, move, moves);
		}
	}

	void appendQueenMoves(BoardState& board, Color side, Square sq, std::vector<Move>& moves)
	{
		for (auto& move : getDiagonalMoves(board, side, sq))
		{
			appendIfNotInCheck(board, side, move, moves);
		}

		for (auto& move : getStraightMoves(board, side, sq))
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
			Move move(board.getPiece(sq), sq, sq + offs, board.getEnPassantSquare());
			signalMayProhibitCastelingKingOrRookMove(board, side, true, true, move);
			appendIfNotInCheck(board, side, move, moves);
		}
		else if (isOpponent(other))
		{
			Move move(board.getPiece(sq), sq, sq + offs, board.getEnPassantSquare(), other, sq + offs);
			signalMayProhibitCastelingKingOrRookMove(board, side, true, true, move);
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
				pieces[squares::h1] == wR && !isSquareInCheck(board, side, squares::f1))
			{
				Move move(pieces[sq], squares::e1, squares::g1, board.getEnPassantSquare());
				signalMayProhibitCastelingKingOrRookMove(board, side, true, true, move);
				appendIfNotInCheck(board, side, move, moves);
			}
			if (castlingAvailability['Q'] && pieces[squares::b1] == none && pieces[squares::c1] == none &&
				pieces[squares::d1] == none && pieces[squares::a1] == wR && !isSquareInCheck(board, side, squares::d1))
			{
				Move move(pieces[sq], squares::e1, squares::c1, board.getEnPassantSquare());
				signalMayProhibitCastelingKingOrRookMove(board, side, true, true, move);
				appendIfNotInCheck(board, side, move, moves);
			}
		}
		else
		{
			if (castlingAvailability['k'] && pieces[squares::f8] == none && pieces[squares::g8] == none &&
				pieces[squares::h8] == bR && !isSquareInCheck(board, side, squares::f8))
			{
				Move move(pieces[sq], squares::e8, squares::g8, board.getEnPassantSquare());
				signalMayProhibitCastelingKingOrRookMove(board, side, true, true, move);
				appendIfNotInCheck(board, side, move, moves);
			}
			if (castlingAvailability['q'] && pieces[squares::b8] == none && pieces[squares::c8] == none &&
				pieces[squares::d8] == none && pieces[squares::a8] == bR && !isSquareInCheck(board, side, squares::d8))
			{
				Move move(pieces[sq], squares::e8, squares::c8, board.getEnPassantSquare());
				signalMayProhibitCastelingKingOrRookMove(board, side, true, true, move);
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

	std::vector<Move> getLegalWhiteMoves(BoardState& board, const FastSqLookup& fastSqLookup, bool debug_doFast)
	{
		static constexpr size_t probableMax = 50;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
			assert(EngineUtilities::isNonNoneSquare(sq));

			switch (board.getPiece(sq))
			{ 
				case wP:
					if (debug_doFast)
						fast_appendWhitePawnMoves(board, sq, fastSqLookup, moves);
					else
						appendWhitePawnMoves(board, sq, moves);
					break;
				case wN:
					if (debug_doFast)
						fast_appendWhiteKnightMoves(board, sq, fastSqLookup, moves);
					else
						appendKnightMoves(board, Color::WHITE, sq, moves);
					break;
				case wB:
					if (debug_doFast)
						fast_appendWhiteBishopMoves(board, sq, fastSqLookup, moves);
					else
						appendBishopMoves(board, Color::WHITE, sq, moves);
					break;
				case wR:
					if (debug_doFast)
						fast_appendWhiteRookMoves(board, sq, fastSqLookup, moves);
					else
						appendRookMoves(board, Color::WHITE, sq, moves);
					break;
				case wQ:
					if (debug_doFast)
						fast_appendWhiteQueenMoves(board, sq, fastSqLookup, moves);
					else
						appendQueenMoves(board, Color::WHITE, sq, moves);
					break;
				case wK:
					if (debug_doFast)
						fast_appendWhiteKingMoves(board, sq, fastSqLookup, moves);
					else
						appendKingMoves(board, Color::WHITE, sq, moves);
					break;
			}
		}

		return moves;
	}

	std::vector<Move> getLegalBlackMoves(BoardState& board, const FastSqLookup& fastSqLookup, bool debug_doFast)
	{
		static constexpr size_t probableMax = 70;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
			assert(EngineUtilities::isNonNoneSquare(sq));

			switch (board.getPiece(sq))
			{
				case bP:
					if (debug_doFast)
						fast_appendBlackPawnMoves(board, sq, fastSqLookup, moves);
					else
						appendBlackPawnMoves(board, sq, moves);
					break;
				case bN:
					if (debug_doFast)
						fast_appendBlackKnightMoves(board, sq, fastSqLookup, moves);
					else
						appendKnightMoves(board, Color::BLACK, sq, moves);
					break;
				case bB:
					if (debug_doFast)
						fast_appendBlackBishopMoves(board, sq, fastSqLookup, moves);
					else
						appendBishopMoves(board, Color::BLACK, sq, moves);
					break;
				case bR:
					if (debug_doFast)
						fast_appendBlackRookMoves(board, sq, fastSqLookup, moves);
					else
						appendRookMoves(board, Color::BLACK, sq, moves);
					break;
				case bQ:
					if (debug_doFast)
						fast_appendBlackQueenMoves(board, sq, fastSqLookup, moves);
					else
						appendQueenMoves(board, Color::BLACK, sq, moves);
					break;
				case bK:
					if (debug_doFast)
						fast_appendBlackKingMoves(board, sq, fastSqLookup, moves);
					else
						appendKingMoves(board, Color::BLACK, sq, moves);
					break;
			}
		}

		return moves;
	}
}

std::vector<Move> Engine::getLegalMoves(BoardState& board) const
{
	//TODO remove do_fast stuff
	constexpr bool doFast = true;

	if (board.getTurn() == pieces::Color::WHITE)
	{
		return getLegalWhiteMoves(board, fastSqLookup, doFast);
	}
	else
	{
		return getLegalBlackMoves(board, fastSqLookup, doFast);
	}
}
