#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Move.h"
#include "PrivateInclude/BoardState.h"
#include "PrivateInclude/EngineUtilities.h"

#include <algorithm>
#include <cassert>

namespace
{
	Piece fast_getPieceAtEndOfSweep(const BoardState& board, const std::vector<Square>& sweep)
	{
		for (const Square sq : sweep)
		{
			const Piece other = board.getPiece(sq);
			if (other != pieces::none)
			{
				assert(EngineUtilities::isNonNonePiece(other));
				return other;
		}
	}

		return pieces::none;
	}

	bool fast_isSquareReachableByBlack(const BoardState& board, Square sq,
		const FastSqLookup& fastSqLookup)
	{
		// Check diagonals (not pawns or king though, they are checked futher below).
		const Piece dA8 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA8()[sq]);
		if (EngineUtilities::isBlack(dA8) && (dA8 == pieces::bQ || dA8 == pieces::bB)) { return true; }

		const Piece dH8 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH8()[sq]);
		if (EngineUtilities::isBlack(dH8) && (dH8 == pieces::bQ || dH8 == pieces::bB)) { return true; }

		const Piece dH1 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH1()[sq]);
		if (EngineUtilities::isBlack(dH1) && (dH1 == pieces::bQ || dH1 == pieces::bB)) { return true; }

		const Piece dA1 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA1()[sq]);
		if (EngineUtilities::isBlack(dA1) && (dA1 == pieces::bQ || dA1 == pieces::bB)) { return true; }

		// Check straights.
		const Piece sRMin = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMin()[sq]);
		if (EngineUtilities::isBlack(sRMin) && (sRMin == pieces::bQ || sRMin == pieces::bR)) { return true; }

		const Piece sRMax = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMax()[sq]);
		if (EngineUtilities::isBlack(sRMax) && (sRMax == pieces::bQ || sRMax == pieces::bR)) { return true; }

		const Piece sFmin = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMin()[sq]);
		if (EngineUtilities::isBlack(sFmin) && (sFmin == pieces::bQ || sFmin == pieces::bR)) { return true; }

		const Piece sFMax = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMax()[sq]);
		if (EngineUtilities::isBlack(sFMax) && (sFMax == pieces::bQ || sFMax == pieces::bR)) { return true; }

		// Check pawns (it is not an error that we use the same color pawn captures here).
		for (const Square pSq : fastSqLookup.getWhitePawnCaptureSquares()[sq])
		{
			if (board.getPiece(pSq) == pieces::bP) { return true; }
		}

		// Check king.
		for (const Square kSq : fastSqLookup.getkingNonCastlingReachableSquares()[sq])
		{
			if (board.getPiece(kSq) == pieces::bK) { return true; }
		}

		// Check knights.
		for (const Square nSq : fastSqLookup.getknightReachableSquares()[sq])
		{
			if (board.getPiece(nSq) == pieces::bN) { return true; }
		}

		return false;
	}

	bool fast_isSquareReachableByWhite(const BoardState& board, Square sq,
		const FastSqLookup& fastSqLookup)
	{
		// Check diagonals (not pawns or king though, they are checked futher below).
		const Piece dA8 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA8()[sq]);
		if (EngineUtilities::isWhite(dA8) && (dA8 == pieces::wQ || dA8 == pieces::wB)) { return true; }

		const Piece dH8 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH8()[sq]);
		if (EngineUtilities::isWhite(dH8) && (dH8 == pieces::wQ || dH8 == pieces::wB)) { return true; }

		const Piece dH1 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH1()[sq]);
		if (EngineUtilities::isWhite(dH1) && (dH1 == pieces::wQ || dH1 == pieces::wB)) { return true; }

		const Piece dA1 = fast_getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA1()[sq]);
		if (EngineUtilities::isWhite(dA1) && (dA1 == pieces::wQ || dA1 == pieces::wB)) { return true; }

		// Check straights.
		const Piece sRMin = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMin()[sq]);
		if (EngineUtilities::isWhite(sRMin) && (sRMin == pieces::wQ || sRMin == pieces::wR)) { return true; }

		const Piece sRMax = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMax()[sq]);
		if (EngineUtilities::isWhite(sRMax) && (sRMax == pieces::wQ || sRMax == pieces::wR)) { return true; }

		const Piece sFmin = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMin()[sq]);
		if (EngineUtilities::isWhite(sFmin) && (sFmin == pieces::wQ || sFmin == pieces::wR)) { return true; }

		const Piece sFMax = fast_getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMax()[sq]);
		if (EngineUtilities::isWhite(sFMax) && (sFMax == pieces::wQ || sFMax == pieces::wR)) { return true; }

		// Check pawns (it is not an error that we use the same color pawn captures here).
		for (const Square pSq : fastSqLookup.getBlackPawnCaptureSquares()[sq])
		{
			if (board.getPiece(pSq) == pieces::wP) { return true; }
		}

		// Check king.
		for (const Square kSq : fastSqLookup.getkingNonCastlingReachableSquares()[sq])
		{
			if (board.getPiece(kSq) == pieces::wK) { return true; }
		}

		// Check knights.
		for (const Square nSq : fastSqLookup.getknightReachableSquares()[sq])
		{
			if (board.getPiece(nSq) == pieces::wN) { return true; }
		}

		return false;
	}

	bool fast_isWhiteInCheck(const BoardState& board, const FastSqLookup& fastSqLookup)
	{
		const auto& pieces = board.getPieces();
		const auto itr = std::find(pieces.begin(), pieces.end(), pieces::wK);
		assert(itr != pieces.end()); // A king must be present on the board.
		const Square kSq = std::distance(pieces.begin(), itr);
		assert(board.getPiece(kSq) == pieces::wK);

		return fast_isSquareReachableByBlack(board, kSq, fastSqLookup);
	}

	bool fast_isBlackInCheck(const BoardState& board, const FastSqLookup& fastSqLookup)
	{
		const auto& pieces = board.getPieces();
		const auto itr = std::find(pieces.begin(), pieces.end(), pieces::bK);
		assert(itr != pieces.end()); // A king must be present on the board.
		const Square kSq = std::distance(pieces.begin(), itr);
		assert(board.getPiece(kSq) == pieces::bK);

		return fast_isSquareReachableByWhite(board, kSq, fastSqLookup);
	}

	void fast_appendIfNotInCheck(BoardState& board, pieces::Color side, const Move& move, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
#ifndef NDEBUG
		const BoardState boardPreMove = board;
#endif

		// Temporarily make the move, before looking if in check.
		auto& pieces = board.getPieces();
		pieces[move.fromSquare] = pieces::none;
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = pieces::none;
		}
		
		pieces[move.toSquare] = move.movingPiece;

		// Handle rook move if casteling.
		if (side == pieces::Color::WHITE)
		{
			if (move.movingPiece == pieces::wK && move.fromSquare == squares::e1)
			{
				if (move.toSquare == squares::g1)
				{
					pieces[squares::h1] = pieces::none;
					pieces[squares::f1] = pieces::wR;
				}
				else if (move.toSquare == squares::c1)
				{
					pieces[squares::a1] = pieces::none;
					pieces[squares::d1] = pieces::wR;
				}
			}
		}
		else
		{
			if (move.movingPiece == pieces::bK && move.fromSquare == squares::e8)
			{
				if (move.toSquare == squares::g8)
				{
					pieces[squares::h8] = pieces::none;
					pieces[squares::f8] = pieces::bR;
				}
				else if (move.toSquare == squares::c8)
				{
					pieces[squares::a8] = pieces::none;
					pieces[squares::d8] = pieces::bR;
				}
			}
		}

		if (side == pieces::Color::WHITE)
		{
			if (!fast_isWhiteInCheck(board, fastSqLookup))
			{
				moves.push_back(move);
			}
		}
		else
		{
			if (!fast_isBlackInCheck(board, fastSqLookup))
			{
				moves.push_back(move);
			}
		}

		// Undo temporary move.
		pieces[move.toSquare] = pieces::none;
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = move.capturedPiece;
		}

		pieces[move.fromSquare] = move.movingPiece;

		// Undo rook move when casteling if this was a casteling move.
		if (side == pieces::Color::WHITE)
		{
			if (move.movingPiece == pieces::wK && move.fromSquare == squares::e1)
			{
				if (move.toSquare == squares::g1)
				{
					pieces[squares::h1] = pieces::wR;
					pieces[squares::f1] = pieces::none;
				}
				else if (move.toSquare == squares::c1)
				{
					pieces[squares::a1] = pieces::wR;
					pieces[squares::d1] = pieces::none;
				}
			}
		}
		else
		{
			if (move.movingPiece == pieces::bK && move.fromSquare == squares::e8)
			{
				if (move.toSquare == squares::g8)
				{
					pieces[squares::h8] = pieces::bR;
					pieces[squares::f8] = pieces::none;
				}
				else if (move.toSquare == squares::c8)
				{
					pieces[squares::a8] = pieces::bR;
					pieces[squares::d8] = pieces::none;
				}
			}
		}

#ifndef NDEBUG
		assert(boardPreMove.getPieces() == board.getPieces());
		assert(boardPreMove.getCastleAvailability() == board.getCastleAvailability());
		assert(boardPreMove.getEnPassantSquare() == board.getEnPassantSquare());
		assert(boardPreMove.getTurn() == board.getTurn());
#endif
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
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(EngineUtilities::isWhite(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				fast_appendIfNotInCheck(board, pieces::Color::WHITE,
					fast_createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				fast_appendIfNotInCheck(board, pieces::Color::WHITE,
					fast_createWhiteRegularCapturingMove(board, sq, nextSq), moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendBlackRegularSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(EngineUtilities::isBlack(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				fast_appendIfNotInCheck(board, pieces::Color::BLACK,
					fast_createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				fast_appendIfNotInCheck(board, pieces::Color::BLACK,
					fast_createBlackRegularCapturingMove(board, sq, nextSq), moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendWhiteRegularNonSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(EngineUtilities::isWhite(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				fast_appendIfNotInCheck(board, pieces::Color::WHITE,
					fast_createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				fast_appendIfNotInCheck(board, pieces::Color::WHITE,
					fast_createWhiteRegularCapturingMove(board, sq, nextSq), moves, fastSqLookup);
			}
		}
	}

	void fast_appendBlackRegularNonSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(EngineUtilities::isBlack(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				fast_appendIfNotInCheck(board, pieces::Color::BLACK,
					fast_createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				fast_appendIfNotInCheck(board, pieces::Color::BLACK,
					fast_createBlackRegularCapturingMove(board, sq, nextSq), moves, fastSqLookup);
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
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
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
				fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = fast_createWhiteRegularCapturingMove(board, sq, nextSq);
				move.prohibitsWKcastling = prohibitsWKcastle;
				move.prohibitsWQcastling = prohibitsWQcastle;
				fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendBlackRookSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
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
				fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = fast_createBlackRegularCapturingMove(board, sq, nextSq);
				move.prohibitsBKcastling = prohibitsBKcastle;
				move.prohibitsBQcastling = prohibitsBQcastle;
				fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void fast_appendWhitePawnAdvanceWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);

		for (const auto promotion : {pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = fast_createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
		}
	}

	void fast_appendBlackPawnAdvanceWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = fast_createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
		}
	}

	void fast_appendWhitePawnCaptureWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);
		assert(EngineUtilities::isBlack(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = fast_createWhiteRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
		}
	}

	void fast_appendBlackPawnCaptureWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);
		assert(EngineUtilities::isWhite(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = fast_createBlackRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
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
				fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = fast_createWhiteRegularCapturingMove(board, sq, toSq);
				move.prohibitsWKcastling = castleKAvailable;
				move.prohibitsWQcastling = castleQAvailable;
				fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
			}
		}

		// Castling moves.
		if (sq == squares::e1 && board.getPiece(squares::f1) == pieces::none &&
			board.getPiece(squares::g1) == pieces::none && castleKAvailable &&
			!fast_isSquareReachableByBlack(board, squares::f1, fastSqLookup))
		{
			assert(board.getPiece(squares::h1) == pieces::wR);
			Move move = fast_createRegularSilentMove(board, sq, squares::g1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
		}

		if (sq == squares::e1 && board.getPiece(squares::b1) == pieces::none &&
			board.getPiece(squares::c1) == pieces::none && board.getPiece(squares::d1) == pieces::none
			&& castleQAvailable && !fast_isSquareReachableByBlack(board, squares::d1, fastSqLookup))
		{
			assert(board.getPiece(squares::a1) == pieces::wR);
			Move move = fast_createRegularSilentMove(board, sq, squares::c1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
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
				fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = fast_createBlackRegularCapturingMove(board, sq, toSq);
				move.prohibitsBKcastling = castleKAvailable;
				move.prohibitsBQcastling = castleQAvailable;
				fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
			}
		}

		// Castling moves.
		if (sq == squares::e8 && board.getPiece(squares::f8) == pieces::none &&
			board.getPiece(squares::g8) == pieces::none && castleKAvailable &&
			!fast_isSquareReachableByWhite(board, squares::f8, fastSqLookup))
		{
			assert(board.getPiece(squares::h8) == pieces::bR);
			Move move = fast_createRegularSilentMove(board, sq, squares::g8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
		}

		if (sq == squares::e8 && board.getPiece(squares::b8) == pieces::none &&
			board.getPiece(squares::c8) == pieces::none && board.getPiece(squares::d8) == pieces::none
			&& castleQAvailable && !fast_isSquareReachableByWhite(board, squares::d8, fastSqLookup))
		{
			assert(board.getPiece(squares::a8) == pieces::bR);
			Move move = fast_createRegularSilentMove(board, sq, squares::c8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
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
				fast_appendWhitePawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves, fastSqLookup);
			}
			else
			{
				fast_appendIfNotInCheck(board, pieces::Color::WHITE, fast_createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, fastSqLookup);
			}
				

			// Double pawn advance.
			const Square doubleAdvanceSq = sq + 16;
			if (rank == 1 && board.getPiece(doubleAdvanceSq) == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				fast_appendIfNotInCheck(board, pieces::Color::WHITE, move, moves, fastSqLookup);
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
					fast_appendWhitePawnCaptureWithPromotions(board, sq, captureSq, moves, fastSqLookup);
				}
				else
				{
					fast_appendIfNotInCheck(board, pieces::Color::WHITE,
						fast_createWhiteRegularCapturingMove(board, sq, captureSq), moves, fastSqLookup);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq - 8) == pieces::bP);
				fast_appendIfNotInCheck(board, pieces::Color::WHITE, Move(pieces::wP, sq,
					captureSq, captureSq, pieces::bP, captureSq - 8), moves, fastSqLookup);
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
				fast_appendBlackPawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves, fastSqLookup);
			}
			else
			{
				fast_appendIfNotInCheck(board, pieces::Color::BLACK, fast_createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, fastSqLookup);
			}


			// Double pawn advance.
			const Square doubleAdvanceSq = sq - 16;
			if (rank == 6 && board.getPiece(doubleAdvanceSq) == pieces::none)
			{
				Move move = fast_createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				fast_appendIfNotInCheck(board, pieces::Color::BLACK, move, moves, fastSqLookup);
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
					fast_appendBlackPawnCaptureWithPromotions(board, sq, captureSq, moves, fastSqLookup);
				}
				else
				{
					fast_appendIfNotInCheck(board, pieces::Color::BLACK,
						fast_createBlackRegularCapturingMove(board, sq, captureSq), moves, fastSqLookup);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq + 8) == pieces::wP);
				fast_appendIfNotInCheck(board, pieces::Color::BLACK, Move(pieces::bP, sq, captureSq,
					captureSq, pieces::wP, captureSq + 8), moves, fastSqLookup);
			}
		}
	}

	void fast_appendWhiteBishopMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
	}

	void fast_appendBlackBishopMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
	}

	void fast_appendWhiteKnightMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRegularNonSweepMoves(board, sq, fastSqLookup.getknightReachableSquares(), moves, fastSqLookup);
	}

	void fast_appendBlackKnightMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRegularNonSweepMoves(board, sq, fastSqLookup.getknightReachableSquares(), moves, fastSqLookup);
	}

	void fast_appendWhiteRookMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves, fastSqLookup);
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves, fastSqLookup);
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves, fastSqLookup);
		fast_appendWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves, fastSqLookup);
	}

	void fast_appendBlackRookMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves, fastSqLookup);
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves, fastSqLookup);
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves, fastSqLookup);
		fast_appendBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves, fastSqLookup);
	}

	void fast_appendWhiteQueenMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves, fastSqLookup);
		fast_appendWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves, fastSqLookup);
	}

	void fast_appendBlackQueenMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves, fastSqLookup);
		fast_appendBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves, fastSqLookup);
	}

	std::vector<Move> getLegalWhiteMoves(BoardState& board, const FastSqLookup& fastSqLookup)
	{
		static constexpr size_t probableMax = 80;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
			assert(EngineUtilities::isNonNoneSquare(sq));

			switch (board.getPiece(sq))
			{ 
				case pieces::wP:
					fast_appendWhitePawnMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wN:
					fast_appendWhiteKnightMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wB:
					fast_appendWhiteBishopMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wR:
					fast_appendWhiteRookMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wQ:
					fast_appendWhiteQueenMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wK:
					fast_appendWhiteKingMoves(board, sq, fastSqLookup, moves);
					break;
			}
		}

		return moves;
	}

	std::vector<Move> getLegalBlackMoves(BoardState& board, const FastSqLookup& fastSqLookup)
	{
		static constexpr size_t probableMax = 80;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
			assert(EngineUtilities::isNonNoneSquare(sq));

			switch (board.getPiece(sq))
			{
				case pieces::bP:
					fast_appendBlackPawnMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bN:
					fast_appendBlackKnightMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bB:
					fast_appendBlackBishopMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bR:
					fast_appendBlackRookMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bQ:
					fast_appendBlackQueenMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bK:
					fast_appendBlackKingMoves(board, sq, fastSqLookup, moves);
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
		return getLegalWhiteMoves(board, fastSqLookup);
	}
	else
	{
		return getLegalBlackMoves(board, fastSqLookup);
	}
}
