#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Move.h"
#include "PrivateInclude/BoardState.h"
#include "PrivateInclude/EngineUtilities.h"

#include <algorithm>
#include <cassert>

namespace moveGenerationHelpers
{
	Piece getPieceAtEndOfSweep(const BoardState& board, const std::vector<Square>& sweep)
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

	bool isSquareReachableByBlack(const BoardState& board, Square sq,
		const FastSqLookup& fastSqLookup)
	{
		// Check diagonals (not pawns or king though, they are checked futher below).
		const Piece dA8 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA8()[sq]);
		if (EngineUtilities::isBlack(dA8) && (dA8 == pieces::bQ || dA8 == pieces::bB)) { return true; }

		const Piece dH8 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH8()[sq]);
		if (EngineUtilities::isBlack(dH8) && (dH8 == pieces::bQ || dH8 == pieces::bB)) { return true; }

		const Piece dH1 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH1()[sq]);
		if (EngineUtilities::isBlack(dH1) && (dH1 == pieces::bQ || dH1 == pieces::bB)) { return true; }

		const Piece dA1 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA1()[sq]);
		if (EngineUtilities::isBlack(dA1) && (dA1 == pieces::bQ || dA1 == pieces::bB)) { return true; }

		// Check straights.
		const Piece sRMin = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMin()[sq]);
		if (EngineUtilities::isBlack(sRMin) && (sRMin == pieces::bQ || sRMin == pieces::bR)) { return true; }

		const Piece sRMax = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMax()[sq]);
		if (EngineUtilities::isBlack(sRMax) && (sRMax == pieces::bQ || sRMax == pieces::bR)) { return true; }

		const Piece sFmin = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMin()[sq]);
		if (EngineUtilities::isBlack(sFmin) && (sFmin == pieces::bQ || sFmin == pieces::bR)) { return true; }

		const Piece sFMax = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMax()[sq]);
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

	bool isSquareReachableByWhite(const BoardState& board, Square sq,
		const FastSqLookup& fastSqLookup)
	{
		// Check diagonals (not pawns or king though, they are checked futher below).
		const Piece dA8 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA8()[sq]);
		if (EngineUtilities::isWhite(dA8) && (dA8 == pieces::wQ || dA8 == pieces::wB)) { return true; }

		const Piece dH8 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH8()[sq]);
		if (EngineUtilities::isWhite(dH8) && (dH8 == pieces::wQ || dH8 == pieces::wB)) { return true; }

		const Piece dH1 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsH1()[sq]);
		if (EngineUtilities::isWhite(dH1) && (dH1 == pieces::wQ || dH1 == pieces::wB)) { return true; }

		const Piece dA1 = getPieceAtEndOfSweep(board, fastSqLookup.getDiagTowardsA1()[sq]);
		if (EngineUtilities::isWhite(dA1) && (dA1 == pieces::wQ || dA1 == pieces::wB)) { return true; }

		// Check straights.
		const Piece sRMin = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMin()[sq]);
		if (EngineUtilities::isWhite(sRMin) && (sRMin == pieces::wQ || sRMin == pieces::wR)) { return true; }

		const Piece sRMax = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsRankMax()[sq]);
		if (EngineUtilities::isWhite(sRMax) && (sRMax == pieces::wQ || sRMax == pieces::wR)) { return true; }

		const Piece sFmin = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMin()[sq]);
		if (EngineUtilities::isWhite(sFmin) && (sFmin == pieces::wQ || sFmin == pieces::wR)) { return true; }

		const Piece sFMax = getPieceAtEndOfSweep(board, fastSqLookup.getstraightTowardsFileMax()[sq]);
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

	bool isWhiteInCheck(const BoardState& board, const FastSqLookup& fastSqLookup)
	{
		const auto& pieces = board.getPieces();
		const auto itr = std::find(pieces.begin(), pieces.end(), pieces::wK);
		assert(itr != pieces.end()); // A king must be present on the board.
		const Square kSq = std::distance(pieces.begin(), itr);
		assert(board.getPiece(kSq) == pieces::wK);

		return isSquareReachableByBlack(board, kSq, fastSqLookup);
	}

	bool isBlackInCheck(const BoardState& board, const FastSqLookup& fastSqLookup)
	{
		const auto& pieces = board.getPieces();
		const auto itr = std::find(pieces.begin(), pieces.end(), pieces::bK);
		assert(itr != pieces.end()); // A king must be present on the board.
		const Square kSq = std::distance(pieces.begin(), itr);
		assert(board.getPiece(kSq) == pieces::bK);

		return isSquareReachableByWhite(board, kSq, fastSqLookup);
	}

	// Ensures that the moving side is not in check priot to adding the move.
	void addWhiteIfValid(BoardState& board, const Move& move,
		std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
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

		if (!isWhiteInCheck(board, fastSqLookup))
		{
			moves.push_back(move);
		}


		// Undo temporary move.
		pieces[move.toSquare] = pieces::none;
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = move.capturedPiece;
		}

		pieces[move.fromSquare] = move.movingPiece;

		// Undo rook move when casteling if this was a casteling move.
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

#ifndef NDEBUG
		assert(boardPreMove.getPieces() == board.getPieces());
		assert(boardPreMove.getCastleAvailability() == board.getCastleAvailability());
		assert(boardPreMove.getEnPassantSquare() == board.getEnPassantSquare());
		assert(boardPreMove.getTurn() == board.getTurn());
#endif
	}

	// Ensures that the moving side is not in check priot to adding the move.
	void addBlackIfValid(BoardState& board, const Move& move,
		std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
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

		if (!isBlackInCheck(board, fastSqLookup))
		{
			moves.push_back(move);
		}

		// Undo temporary move.
		pieces[move.toSquare] = pieces::none;
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = move.capturedPiece;
		}

		pieces[move.fromSquare] = move.movingPiece;

		// Undo rook move when casteling if this was a casteling move.
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

#ifndef NDEBUG
		assert(boardPreMove.getPieces() == board.getPieces());
		assert(boardPreMove.getCastleAvailability() == board.getCastleAvailability());
		assert(boardPreMove.getEnPassantSquare() == board.getEnPassantSquare());
		assert(boardPreMove.getTurn() == board.getTurn());
#endif
	}

	bool rookCaptureProhibitsWQcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (captureSquare == squares::a1 && board.getCastleAvailability().find('Q')->second)
		{
			return true;
		}

		return false;
	}

	bool rookCaptureProhibitsWKcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (captureSquare == squares::h1 && board.getCastleAvailability().find('K')->second)
		{
			return true;
		}

		return false;
	}

	bool rookCaptureProhibitsBQcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (captureSquare == squares::a8 && board.getCastleAvailability().find('q')->second)
		{
			return true;
		}

		return false;
	}

	bool rookCaptureProhibitsBKcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (captureSquare == squares::h8 && board.getCastleAvailability().find('k')->second)
		{
			return true;
		}

		return false;
	}


	// Note: Does not handle en passant square creation, pawn promotion, casteling or
	// castling avaialibity.
	Move createRegularSilentMove(const BoardState& board, Square fromSquare, Square toSquare)
	{
		return Move(board.getPiece(fromSquare), fromSquare, toSquare, board.getEnPassantSquare());
	}

	// Note: Does not handle pawn promotion, en passant captures or castling avaialibity
	// due to rook or king moved. Does handle castling availability due to captured rook though.
	Move createWhiteRegularCapturingMove(const BoardState& board, Square fromSquare, Square toSquare)
	{
		assert(board.getTurn() == pieces::Color::WHITE);

		const Piece capturedPiece = board.getPiece(toSquare);
		Move move(board.getPiece(fromSquare), fromSquare, toSquare, board.getEnPassantSquare(),
			capturedPiece, toSquare);

		// Handle castling availability if a rook was captured.
		if (capturedPiece == pieces::bR)
		{
			if (rookCaptureProhibitsBKcastling(board, toSquare))
			{
				move.prohibitsBKcastling = true;
			}
			else if (rookCaptureProhibitsBQcastling(board, toSquare))
			{
				move.prohibitsBQcastling = true;
			}
		}

		return move;
	}

	// Note: Does not handle pawn promotion, en passant captures or castling avaialibity
	// due to rook or king moved. Does handle castling availability due to captured rook though.
	Move createBlackRegularCapturingMove(const BoardState& board, Square fromSquare, Square toSquare)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		const Piece capturedPiece = board.getPiece(toSquare);
		Move move(board.getPiece(fromSquare), fromSquare, toSquare, board.getEnPassantSquare(),
			capturedPiece, toSquare);

		// Handle castling availability if a rook was captured.
		if (capturedPiece == pieces::wR)
		{
			if (rookCaptureProhibitsWKcastling(board, toSquare))
			{
				move.prohibitsWKcastling = true;
			}
			else if (rookCaptureProhibitsWQcastling(board, toSquare))
			{
				move.prohibitsWQcastling = true;
			}
		}

		return move;
	}

	void addWhiteRegularSweepMoves(BoardState& board, Square sq,
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
				addWhiteIfValid(board, createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				addWhiteIfValid(board, createWhiteRegularCapturingMove(board, sq, nextSq),
					moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void addBlackRegularSweepMoves(BoardState& board, Square sq,
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
				addBlackIfValid(board, createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				addBlackIfValid(board, createBlackRegularCapturingMove(board, sq, nextSq),
					moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void addWhiteRegularNonSweepMoves(BoardState& board, Square sq,
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
				addWhiteIfValid(board, createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				addWhiteIfValid(board, createWhiteRegularCapturingMove(board, sq, nextSq),
					moves, fastSqLookup);
			}
		}
	}

	void addBlackRegularNonSweepMoves(BoardState& board, Square sq,
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
				addBlackIfValid(board,
					createRegularSilentMove(board, sq, nextSq), moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				addBlackIfValid(board,
					createBlackRegularCapturingMove(board, sq, nextSq), moves, fastSqLookup);
			}
		}
	}

	bool rookMoveProhibitsWKCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (fromSquare == squares::h1 && board.getCastleAvailability().find('K')->second)
		{
			return true;
		}

		return false;
	}

	bool rookMoveProhibitsWQCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::WHITE);
		if (fromSquare == squares::a1 && board.getCastleAvailability().find('Q')->second)
		{
			return true;
		}

		return false;
	}

	bool rookMoveProhibitsBKCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (fromSquare == squares::h8 && board.getCastleAvailability().find('k')->second)
		{
			return true;
		}

		return false;
	}

	bool rookMoveProhibitsBQCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::BLACK);
		if (fromSquare == squares::a8 && board.getCastleAvailability().find('q')->second)
		{
			return true;
		}

		return false;
	}

	void addWhiteRookSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(board.getPiece(sq) == pieces::wR);
		const bool prohibitsWKcastle = rookMoveProhibitsWKCastling(board, sq);
		const bool prohibitsWQcastle = rookMoveProhibitsWQCastling(board, sq);
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				Move move = createRegularSilentMove(board, sq, nextSq);
				move.prohibitsWKcastling = prohibitsWKcastle;
				move.prohibitsWQcastling = prohibitsWQcastle;
				addWhiteIfValid(board, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = createWhiteRegularCapturingMove(board, sq, nextSq);
				move.prohibitsWKcastling = prohibitsWKcastle;
				move.prohibitsWQcastling = prohibitsWQcastle;
				addWhiteIfValid(board, move, moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void addBlackRookSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(board.getPiece(sq) == pieces::bR);
		const bool prohibitsBKcastle = rookMoveProhibitsBKCastling(board, sq);
		const bool prohibitsBQcastle = rookMoveProhibitsBQCastling(board, sq);
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				Move move = createRegularSilentMove(board, sq, nextSq);
				move.prohibitsBKcastling = prohibitsBKcastle;
				move.prohibitsBQcastling = prohibitsBQcastle;
				addBlackIfValid(board, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = createBlackRegularCapturingMove(board, sq, nextSq);
				move.prohibitsBKcastling = prohibitsBKcastle;
				move.prohibitsBQcastling = prohibitsBQcastle;
				addBlackIfValid(board, move, moves, fastSqLookup);
				return;
			}
			else
			{
				return; // Found same colored piece.
			}
		}
	}

	void addWhitePawnAdvanceWithPromotions(BoardState& board,
		Square fromSquare, Square toSquare, std::vector<Move>& moves,
		const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);

		for (const auto promotion : {pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addWhiteIfValid(board, move, moves, fastSqLookup);
		}
	}

	void addBlackPawnAdvanceWithPromotions(BoardState& board, Square fromSquare,
		Square toSquare, std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addBlackIfValid(board, move, moves, fastSqLookup);
		}
	}

	void addWhitePawnCaptureWithPromotions(BoardState& board, Square fromSquare,
		Square toSquare, std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);
		assert(EngineUtilities::isBlack(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = createWhiteRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addWhiteIfValid(board, move, moves, fastSqLookup);
		}
	}

	void addBlackPawnCaptureWithPromotions(BoardState& board, Square fromSquare,
		Square toSquare, std::vector<Move>& moves, const FastSqLookup& fastSqLookup)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);
		assert(EngineUtilities::isWhite(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = createBlackRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addBlackIfValid(board, move, moves, fastSqLookup);
		}
	}

	void addWhiteKingMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
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
				Move move = createRegularSilentMove(board, sq, toSq);
				move.prohibitsWKcastling = castleKAvailable;
				move.prohibitsWQcastling = castleQAvailable;
				addWhiteIfValid(board, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = createWhiteRegularCapturingMove(board, sq, toSq);
				move.prohibitsWKcastling = castleKAvailable;
				move.prohibitsWQcastling = castleQAvailable;
				addWhiteIfValid(board, move, moves, fastSqLookup);
			}
		}

		// Castling moves.
		if (sq == squares::e1 && board.getPiece(squares::f1) == pieces::none &&
			board.getPiece(squares::g1) == pieces::none && castleKAvailable &&
			!isSquareReachableByBlack(board, squares::f1, fastSqLookup) &&
			!isSquareReachableByBlack(board, squares::e1, fastSqLookup))
		{
			assert(board.getPiece(squares::h1) == pieces::wR);
			Move move = createRegularSilentMove(board, sq, squares::g1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			addWhiteIfValid(board, move, moves, fastSqLookup);
		}

		if (sq == squares::e1 && board.getPiece(squares::b1) == pieces::none &&
			board.getPiece(squares::c1) == pieces::none && board.getPiece(squares::d1) == pieces::none
			&& castleQAvailable && !isSquareReachableByBlack(board, squares::d1, fastSqLookup)
			&& !isSquareReachableByBlack(board, squares::e1, fastSqLookup))
		{
			assert(board.getPiece(squares::a1) == pieces::wR);
			Move move = createRegularSilentMove(board, sq, squares::c1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			addWhiteIfValid(board, move, moves, fastSqLookup);
		}
	}

	void addBlackKingMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
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
				Move move = createRegularSilentMove(board, sq, toSq);
				move.prohibitsBKcastling = castleKAvailable;
				move.prohibitsBQcastling = castleQAvailable;
				addBlackIfValid(board, move, moves, fastSqLookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = createBlackRegularCapturingMove(board, sq, toSq);
				move.prohibitsBKcastling = castleKAvailable;
				move.prohibitsBQcastling = castleQAvailable;
				addBlackIfValid(board, move, moves, fastSqLookup);
			}
		}

		// Castling moves.
		if (sq == squares::e8 && board.getPiece(squares::f8) == pieces::none &&
			board.getPiece(squares::g8) == pieces::none && castleKAvailable &&
			!isSquareReachableByWhite(board, squares::f8, fastSqLookup) &&
			!isSquareReachableByWhite(board, squares::e8, fastSqLookup))
		{
			assert(board.getPiece(squares::h8) == pieces::bR);
			Move move = createRegularSilentMove(board, sq, squares::g8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			addBlackIfValid(board, move, moves, fastSqLookup);
		}

		if (sq == squares::e8 && board.getPiece(squares::b8) == pieces::none &&
			board.getPiece(squares::c8) == pieces::none && board.getPiece(squares::d8) == pieces::none
			&& castleQAvailable && !isSquareReachableByWhite(board, squares::d8, fastSqLookup) &&
			!isSquareReachableByWhite(board, squares::e8, fastSqLookup))
		{
			assert(board.getPiece(squares::a8) == pieces::bR);
			Move move = createRegularSilentMove(board, sq, squares::c8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			addBlackIfValid(board, move, moves, fastSqLookup);
		}
	}

	void addWhitePawnMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
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
				addWhitePawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves, fastSqLookup);
			}
			else
			{
				addWhiteIfValid(board, createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, fastSqLookup);
			}
				

			// Double pawn advance.
			const Square doubleAdvanceSq = sq + 16;
			if (rank == 1 && board.getPiece(doubleAdvanceSq) == pieces::none)
			{
				Move move = createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				addWhiteIfValid(board, move, moves, fastSqLookup);
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
					addWhitePawnCaptureWithPromotions(board, sq, captureSq, moves, fastSqLookup);
				}
				else
				{
					addWhiteIfValid(board,
						createWhiteRegularCapturingMove(board, sq, captureSq), moves, fastSqLookup);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq - 8) == pieces::bP);
				addWhiteIfValid(board, Move(pieces::wP, sq,
					captureSq, captureSq, pieces::bP, captureSq - 8), moves, fastSqLookup);
			}
		}
	}

	void addBlackPawnMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
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
				addBlackPawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves, fastSqLookup);
			}
			else
			{
				addBlackIfValid(board, createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, fastSqLookup);
			}


			// Double pawn advance.
			const Square doubleAdvanceSq = sq - 16;
			if (rank == 6 && board.getPiece(doubleAdvanceSq) == pieces::none)
			{
				Move move = createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				addBlackIfValid(board, move, moves, fastSqLookup);
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
					addBlackPawnCaptureWithPromotions(board, sq, captureSq, moves, fastSqLookup);
				}
				else
				{
					addBlackIfValid(board,
						createBlackRegularCapturingMove(board, sq, captureSq), moves, fastSqLookup);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq + 8) == pieces::wP);
				addBlackIfValid(board, Move(pieces::bP, sq, captureSq,
					captureSq, pieces::wP, captureSq + 8), moves, fastSqLookup);
			}
		}
	}

	void addWhiteBishopMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
	}

	void addBlackBishopMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
	}

	void addWhiteKnightMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addWhiteRegularNonSweepMoves(board, sq, fastSqLookup.getknightReachableSquares(),
			moves, fastSqLookup);
	}

	void addBlackKnightMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addBlackRegularNonSweepMoves(board, sq, fastSqLookup.getknightReachableSquares(),
			moves, fastSqLookup);
	}

	void addWhiteRookMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves, fastSqLookup);
		addWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves, fastSqLookup);
		addWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves, fastSqLookup);
		addWhiteRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves, fastSqLookup);
	}

	void addBlackRookMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves, fastSqLookup);
		addBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves, fastSqLookup);
		addBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves, fastSqLookup);
		addBlackRookSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves, fastSqLookup);
	}

	void addWhiteQueenMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves,
			fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves,
			fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves,
			fastSqLookup);
		addWhiteRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves,
			fastSqLookup);
	}

	void addBlackQueenMoves(BoardState& board, Square sq, const FastSqLookup& fastSqLookup,
		std::vector<Move>& moves)
	{
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH8(), moves, fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA8(), moves, fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsH1(), moves, fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getDiagTowardsA1(), moves, fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMin(), moves,
			fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsRankMax(), moves,
			fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMin(), moves,
			fastSqLookup);
		addBlackRegularSweepMoves(board, sq, fastSqLookup.getstraightTowardsFileMax(), moves,
			fastSqLookup);
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
					addWhitePawnMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wN:
					addWhiteKnightMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wB:
					addWhiteBishopMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wR:
					addWhiteRookMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wQ:
					addWhiteQueenMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::wK:
					addWhiteKingMoves(board, sq, fastSqLookup, moves);
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
					addBlackPawnMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bN:
					addBlackKnightMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bB:
					addBlackBishopMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bR:
					addBlackRookMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bQ:
					addBlackQueenMoves(board, sq, fastSqLookup, moves);
					break;
				case pieces::bK:
					addBlackKingMoves(board, sq, fastSqLookup, moves);
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
		return moveGenerationHelpers::getLegalWhiteMoves(board, fastSqLookup);
	}
	else
	{
		return moveGenerationHelpers::getLegalBlackMoves(board, fastSqLookup);
	}
}
