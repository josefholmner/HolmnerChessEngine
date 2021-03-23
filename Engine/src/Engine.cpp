#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Move.h"
#include "PrivateInclude/EngineUtilities.h"

#include <cassert>

namespace moveGenerationHelpers
{
	struct Lookup
	{
		Lookup(const FastSqLookup& fastSqLookup) : fSqL{fastSqLookup}{}

		const FastSqLookup& fSqL;
		bool movingSideInCheckPreMove = true;
		bool includeOnlyCapturesAndPromotions = false;
	};


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
		const Lookup& lookup)
	{
		// Check diagonals (not pawns or king though, they are checked futher below).
		const Piece dA8 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsA8()[sq]);
		if (EngineUtilities::isBlack(dA8) && (dA8 == pieces::bQ || dA8 == pieces::bB)) { return true; }

		const Piece dH8 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsH8()[sq]);
		if (EngineUtilities::isBlack(dH8) && (dH8 == pieces::bQ || dH8 == pieces::bB)) { return true; }

		const Piece dH1 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsH1()[sq]);
		if (EngineUtilities::isBlack(dH1) && (dH1 == pieces::bQ || dH1 == pieces::bB)) { return true; }

		const Piece dA1 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsA1()[sq]);
		if (EngineUtilities::isBlack(dA1) && (dA1 == pieces::bQ || dA1 == pieces::bB)) { return true; }

		// Check straights.
		const Piece sRMin = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsRankMin()[sq]);
		if (EngineUtilities::isBlack(sRMin) && (sRMin == pieces::bQ || sRMin == pieces::bR)) { return true; }

		const Piece sRMax = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsRankMax()[sq]);
		if (EngineUtilities::isBlack(sRMax) && (sRMax == pieces::bQ || sRMax == pieces::bR)) { return true; }

		const Piece sFmin = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsFileMin()[sq]);
		if (EngineUtilities::isBlack(sFmin) && (sFmin == pieces::bQ || sFmin == pieces::bR)) { return true; }

		const Piece sFMax = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsFileMax()[sq]);
		if (EngineUtilities::isBlack(sFMax) && (sFMax == pieces::bQ || sFMax == pieces::bR)) { return true; }

		// Check pawns (it is not an error that we use the same color pawn captures here).
		for (const Square pSq : lookup.fSqL.getWhitePawnCaptureSquares()[sq])
		{
			if (board.getPiece(pSq) == pieces::bP) { return true; }
		}

		// Check king.
		for (const Square kSq : lookup.fSqL.getkingNonCastlingReachableSquares()[sq])
		{
			if (board.getPiece(kSq) == pieces::bK) { return true; }
		}

		// Check knights.
		for (const Square nSq : lookup.fSqL.getknightReachableSquares()[sq])
		{
			if (board.getPiece(nSq) == pieces::bN) { return true; }
		}

		return false;
	}

	bool isSquareReachableByWhite(const BoardState& board, Square sq,
		const Lookup& lookup)
	{
		// Check diagonals (not pawns or king though, they are checked futher below).
		const Piece dA8 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsA8()[sq]);
		if (EngineUtilities::isWhite(dA8) && (dA8 == pieces::wQ || dA8 == pieces::wB)) { return true; }

		const Piece dH8 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsH8()[sq]);
		if (EngineUtilities::isWhite(dH8) && (dH8 == pieces::wQ || dH8 == pieces::wB)) { return true; }

		const Piece dH1 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsH1()[sq]);
		if (EngineUtilities::isWhite(dH1) && (dH1 == pieces::wQ || dH1 == pieces::wB)) { return true; }

		const Piece dA1 = getPieceAtEndOfSweep(board, lookup.fSqL.getDiagTowardsA1()[sq]);
		if (EngineUtilities::isWhite(dA1) && (dA1 == pieces::wQ || dA1 == pieces::wB)) { return true; }

		// Check straights.
		const Piece sRMin = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsRankMin()[sq]);
		if (EngineUtilities::isWhite(sRMin) && (sRMin == pieces::wQ || sRMin == pieces::wR)) { return true; }

		const Piece sRMax = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsRankMax()[sq]);
		if (EngineUtilities::isWhite(sRMax) && (sRMax == pieces::wQ || sRMax == pieces::wR)) { return true; }

		const Piece sFmin = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsFileMin()[sq]);
		if (EngineUtilities::isWhite(sFmin) && (sFmin == pieces::wQ || sFmin == pieces::wR)) { return true; }

		const Piece sFMax = getPieceAtEndOfSweep(board, lookup.fSqL.getstraightTowardsFileMax()[sq]);
		if (EngineUtilities::isWhite(sFMax) && (sFMax == pieces::wQ || sFMax == pieces::wR)) { return true; }

		// Check pawns (it is not an error that we use the same color pawn captures here).
		for (const Square pSq : lookup.fSqL.getBlackPawnCaptureSquares()[sq])
		{
			if (board.getPiece(pSq) == pieces::wP) { return true; }
		}

		// Check king.
		for (const Square kSq : lookup.fSqL.getkingNonCastlingReachableSquares()[sq])
		{
			if (board.getPiece(kSq) == pieces::wK) { return true; }
		}

		// Check knights.
		for (const Square nSq : lookup.fSqL.getknightReachableSquares()[sq])
		{
			if (board.getPiece(nSq) == pieces::wN) { return true; }
		}

		return false;
	}

	bool isWhiteInCheck(const BoardState& board, const Lookup& lookup)
	{
		assert(board.getPiece(board.getWhiteKingSquare()) == pieces::wK);
		return isSquareReachableByBlack(board, board.getWhiteKingSquare(), lookup);
	}

	bool isBlackInCheck(const BoardState& board, const Lookup& lookup)
	{
		assert(board.getPiece(board.getBlackKingSquare()) == pieces::bK);
		return isSquareReachableByWhite(board, board.getBlackKingSquare(), lookup);
	}

	// Ensures that the moving side is not in check prior to adding the move.
	void addWhiteIfValid(BoardState& board, const Move& move, std::vector<Move>& moves,
		const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
#ifndef NDEBUG
		const BoardState boardPreMove = board;
#endif

		// Optimization: if the side whos turn it is was not in check before making the move, a
		// move that involves moving a piece that is not in line with its own king prior to making
		// the move cannot cause the moving side to move itself into check. This is true as long as
		// the moving piece is not the king itself or if making an en passant capture. 
		// Simularly, if the moving side was already in check before the move, any non-capturing
		// move with its end square not in line with its own king cannot make the moving side not
		// in check. We use these facts here to not having to do the move to determine if it would
		// cause a check or not.
		if (lookup.movingSideInCheckPreMove)
		{
			if (move.capturedPiece == pieces::none && !lookup.fSqL.areSquaresInLinearLineOfSight(
				move.toSquare, board.getWhiteKingSquare()) && move.movingPiece != pieces::wK)
			{
				// Non-capturing, non-king move ending up not in line of sight of the king cannot
				// make us not in check, so this move is invalid and we return early.
				return;
			}
		}
		else
		{
			if (!lookup.fSqL.areSquaresInLinearLineOfSight(move.fromSquare, board.getWhiteKingSquare())
				&& move.movingPiece != pieces::wK && move.toSquare != board.getEnPassantSquare())
			{
				moves.push_back(move);
				return;
			}
		}

		// Temporarily make the move, before looking if in check.
		auto& pieces = board.getPieces();
		pieces[move.fromSquare] = pieces::none;
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = pieces::none;
		}

		pieces[move.toSquare] = move.movingPiece;

		// Handle rook move if casteling and set king square.
		if (move.movingPiece == pieces::wK)
		{
			board.setWhiteKingSquare(move.toSquare);
			if (move.fromSquare == squares::e1)
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

		if (!isWhiteInCheck(board, lookup))
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
		if (move.movingPiece == pieces::wK)
		{
			board.setWhiteKingSquare(move.fromSquare);
			if (move.fromSquare == squares::e1)
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

#ifndef NDEBUG
		assert(boardPreMove.getPieces() == board.getPieces());
		assert(boardPreMove.getCastleAvailability() == board.getCastleAvailability());
		assert(boardPreMove.getEnPassantSquare() == board.getEnPassantSquare());
		assert(boardPreMove.getTurn() == board.getTurn());
#endif
	}

	// Ensures that the moving side is not in check prior to adding the move.
	void addBlackIfValid(BoardState& board, const Move& move,
		std::vector<Move>& moves, const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
#ifndef NDEBUG
		const BoardState boardPreMove = board;
#endif

		// Optimization: if the side whos turn it is was not in check before making the move, a
		// move that involves moving a piece that is not in line with its own king prior to making
		// the move cannot cause the moving side to move itself into check. This is true as long as
		// the moving piece is not the king itself or if making an en passant capture. 
		// Simularly, if the moving side was already in check before the move, any non-capturing
		// move with its end square not in line with its own king cannot make the moving side not
		// in check. We use these facts here to not having to do the move to determine if it would
		// cause a check or not.
		if (lookup.movingSideInCheckPreMove)
		{
			if (move.capturedPiece == pieces::none && !lookup.fSqL.areSquaresInLinearLineOfSight(
				move.toSquare, board.getBlackKingSquare()) && move.movingPiece != pieces::bK)
			{
				// Non-capturing, non-king move ending up not in line of sight of the king cannot
				// make us not in check, so this move is invalid and we return early.
				return;
			}
		}
		else
		{
			if (!lookup.fSqL.areSquaresInLinearLineOfSight(move.fromSquare, board.getBlackKingSquare())
				&& move.movingPiece != pieces::bK && move.toSquare != board.getEnPassantSquare())
			{
				moves.push_back(move);
				return;
			}
		}

		// Temporarily make the move, before looking if in check.
		auto& pieces = board.getPieces();
		pieces[move.fromSquare] = pieces::none;
		if (move.capturedPiece != pieces::none)
		{
			pieces[move.capturedSquare] = pieces::none;
		}
		
		pieces[move.toSquare] = move.movingPiece;

		// Handle rook move if casteling.
		if (move.movingPiece == pieces::bK)
		{
			board.setBlackKingSquare(move.toSquare);
			if (move.fromSquare == squares::e8)
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

		if (!isBlackInCheck(board, lookup))
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
		if (move.movingPiece == pieces::bK)
		{
			board.setBlackKingSquare(move.fromSquare);
			if (move.fromSquare == squares::e8)
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

	bool rookCaptureProhibitsWQcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::BLACK);
		return captureSquare == squares::a1 && board.getCastleAvailability().find('Q')->second;
	}

	bool rookCaptureProhibitsWKcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::BLACK);
		return captureSquare == squares::h1 && board.getCastleAvailability().find('K')->second;
	}

	bool rookCaptureProhibitsBQcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::WHITE);
		return captureSquare == squares::a8 && board.getCastleAvailability().find('q')->second;
	}

	bool rookCaptureProhibitsBKcastling(const BoardState& board, Square captureSquare)
	{
		assert(board.getPiece(captureSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::WHITE);
		return captureSquare == squares::h8 && board.getCastleAvailability().find('k')->second;
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
		const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(EngineUtilities::isWhite(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				if (!lookup.includeOnlyCapturesAndPromotions)
				{
					addWhiteIfValid(board, createRegularSilentMove(board, sq, nextSq), moves, lookup);
				}
			}
			else if (EngineUtilities::isBlack(other))
			{
				addWhiteIfValid(board, createWhiteRegularCapturingMove(board, sq, nextSq),
					moves, lookup);
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
		const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(EngineUtilities::isBlack(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none)
			{
				if (!lookup.includeOnlyCapturesAndPromotions)
				{
					addBlackIfValid(board, createRegularSilentMove(board, sq, nextSq), moves, lookup);
				}
			}
			else if (EngineUtilities::isWhite(other))
			{
				addBlackIfValid(board, createBlackRegularCapturingMove(board, sq, nextSq),
					moves, lookup);
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
		const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(EngineUtilities::isWhite(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none && !lookup.includeOnlyCapturesAndPromotions)
			{
				addWhiteIfValid(board, createRegularSilentMove(board, sq, nextSq), moves, lookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				addWhiteIfValid(board, createWhiteRegularCapturingMove(board, sq, nextSq),
					moves, lookup);
			}
		}
	}

	void addBlackRegularNonSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(EngineUtilities::isBlack(board.getPiece(sq)));
		for (const Square nextSq : trySqs[sq])
		{
			const Piece other = board.getPiece(nextSq);
			if (other == pieces::none && !lookup.includeOnlyCapturesAndPromotions)
			{
				addBlackIfValid(board, createRegularSilentMove(board, sq, nextSq), moves, lookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				addBlackIfValid(board, createBlackRegularCapturingMove(board, sq, nextSq),
					moves, lookup);
			}
		}
	}

	bool rookMoveProhibitsWKCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::WHITE);
		return fromSquare == squares::h1 && board.getCastleAvailability().find('K')->second;
	}

	bool rookMoveProhibitsWQCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::wR);
		assert(board.getTurn() == pieces::Color::WHITE);
		return fromSquare == squares::a1 && board.getCastleAvailability().find('Q')->second;
	}

	bool rookMoveProhibitsBKCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::BLACK);
		return fromSquare == squares::h8 && board.getCastleAvailability().find('k')->second;
	}

	bool rookMoveProhibitsBQCastling(const BoardState& board, Square fromSquare)
	{
		assert(board.getPiece(fromSquare) == pieces::bR);
		assert(board.getTurn() == pieces::Color::BLACK);
		return fromSquare == squares::a8 && board.getCastleAvailability().find('q')->second;
	}

	void addWhiteRookSweepMoves(BoardState& board, Square sq,
		const std::array<std::vector<Square>, squares::num>& trySqs, std::vector<Move>& moves,
		const Lookup& lookup)
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
				if (!lookup.includeOnlyCapturesAndPromotions)
				{
					Move move = createRegularSilentMove(board, sq, nextSq);
					move.prohibitsWKcastling = prohibitsWKcastle;
					move.prohibitsWQcastling = prohibitsWQcastle;
					addWhiteIfValid(board, move, moves, lookup);
				}
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = createWhiteRegularCapturingMove(board, sq, nextSq);
				move.prohibitsWKcastling = prohibitsWKcastle;
				move.prohibitsWQcastling = prohibitsWQcastle;
				addWhiteIfValid(board, move, moves, lookup);
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
		const Lookup& lookup)
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
				if (!lookup.includeOnlyCapturesAndPromotions)
				{
					Move move = createRegularSilentMove(board, sq, nextSq);
					move.prohibitsBKcastling = prohibitsBKcastle;
					move.prohibitsBQcastling = prohibitsBQcastle;
					addBlackIfValid(board, move, moves, lookup);
				}
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = createBlackRegularCapturingMove(board, sq, nextSq);
				move.prohibitsBKcastling = prohibitsBKcastle;
				move.prohibitsBQcastling = prohibitsBQcastle;
				addBlackIfValid(board, move, moves, lookup);
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
		const Lookup& lookup)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);

		for (const auto promotion : {pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addWhiteIfValid(board, move, moves, lookup);
		}
	}

	void addBlackPawnAdvanceWithPromotions(BoardState& board, Square fromSquare,
		Square toSquare, std::vector<Move>& moves, const Lookup& lookup)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = createRegularSilentMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addBlackIfValid(board, move, moves, lookup);
		}
	}

	void addWhitePawnCaptureWithPromotions(BoardState& board, Square fromSquare,
		Square toSquare, std::vector<Move>& moves, const Lookup& lookup)
	{
		assert(ranks::toRank(fromSquare) == 6);
		assert(ranks::toRank(toSquare) == 7);
		assert(EngineUtilities::isBlack(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::wQ, pieces::wR, pieces::wB, pieces::wN })
		{
			Move move = createWhiteRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addWhiteIfValid(board, move, moves, lookup);
		}
	}

	void addBlackPawnCaptureWithPromotions(BoardState& board, Square fromSquare,
		Square toSquare, std::vector<Move>& moves, const Lookup& lookup)
	{
		assert(ranks::toRank(fromSquare) == 1);
		assert(ranks::toRank(toSquare) == 0);
		assert(EngineUtilities::isWhite(board.getPiece(toSquare)));

		for (const auto promotion : { pieces::bQ, pieces::bR, pieces::bB, pieces::bN })
		{
			Move move = createBlackRegularCapturingMove(board, fromSquare, toSquare);
			move.pawnPromotionPiece = promotion;
			addBlackIfValid(board, move, moves, lookup);
		}
	}

	void addWhiteKingMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		assert(board.getPiece(sq) == pieces::wK);

		const bool castleKAvailable = board.getCastleAvailability().find('K')->second;
		const bool castleQAvailable = board.getCastleAvailability().find('Q')->second;

		// Non castling moves.
		for (const Square toSq : lookup.fSqL.getkingNonCastlingReachableSquares()[sq])
		{
			const Piece other = board.getPiece(toSq);
			if (other == pieces::none && !lookup.includeOnlyCapturesAndPromotions)
			{
				Move move = createRegularSilentMove(board, sq, toSq);
				move.prohibitsWKcastling = castleKAvailable;
				move.prohibitsWQcastling = castleQAvailable;
				addWhiteIfValid(board, move, moves, lookup);
			}
			else if (EngineUtilities::isBlack(other))
			{
				Move move = createWhiteRegularCapturingMove(board, sq, toSq);
				move.prohibitsWKcastling = castleKAvailable;
				move.prohibitsWQcastling = castleQAvailable;
				addWhiteIfValid(board, move, moves, lookup);
			}
		}

		// Castling moves.
		if (sq == squares::e1 && board.getPiece(squares::f1) == pieces::none &&
			board.getPiece(squares::g1) == pieces::none && castleKAvailable &&
			!isSquareReachableByBlack(board, squares::f1, lookup) &&
			!isSquareReachableByBlack(board, squares::e1, lookup) &&
			!lookup.includeOnlyCapturesAndPromotions)
		{
			assert(board.getPiece(squares::h1) == pieces::wR);
			Move move = createRegularSilentMove(board, sq, squares::g1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			addWhiteIfValid(board, move, moves, lookup);
		}

		if (sq == squares::e1 && board.getPiece(squares::b1) == pieces::none &&
			board.getPiece(squares::c1) == pieces::none && board.getPiece(squares::d1) == pieces::none
			&& castleQAvailable && !isSquareReachableByBlack(board, squares::d1, lookup)
			&& !isSquareReachableByBlack(board, squares::e1, lookup)
			&& !lookup.includeOnlyCapturesAndPromotions)
		{
			assert(board.getPiece(squares::a1) == pieces::wR);
			Move move = createRegularSilentMove(board, sq, squares::c1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			addWhiteIfValid(board, move, moves, lookup);
		}
	}

	void addBlackKingMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		assert(board.getPiece(sq) == pieces::bK);
		const bool castleKAvailable = board.getCastleAvailability().find('k')->second;
		const bool castleQAvailable = board.getCastleAvailability().find('q')->second;

		// Non castling moves.
		for (const Square toSq : lookup.fSqL.getkingNonCastlingReachableSquares()[sq])
		{
			const Piece other = board.getPiece(toSq);
			if (other == pieces::none && !lookup.includeOnlyCapturesAndPromotions)
			{
				Move move = createRegularSilentMove(board, sq, toSq);
				move.prohibitsBKcastling = castleKAvailable;
				move.prohibitsBQcastling = castleQAvailable;
				addBlackIfValid(board, move, moves, lookup);
			}
			else if (EngineUtilities::isWhite(other))
			{
				Move move = createBlackRegularCapturingMove(board, sq, toSq);
				move.prohibitsBKcastling = castleKAvailable;
				move.prohibitsBQcastling = castleQAvailable;
				addBlackIfValid(board, move, moves, lookup);
			}
		}

		// Castling moves.
		if (sq == squares::e8 && board.getPiece(squares::f8) == pieces::none &&
			board.getPiece(squares::g8) == pieces::none && castleKAvailable &&
			!isSquareReachableByWhite(board, squares::f8, lookup) &&
			!isSquareReachableByWhite(board, squares::e8, lookup) &&
			!lookup.includeOnlyCapturesAndPromotions)
		{
			assert(board.getPiece(squares::h8) == pieces::bR);
			Move move = createRegularSilentMove(board, sq, squares::g8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			addBlackIfValid(board, move, moves, lookup);
		}

		if (sq == squares::e8 && board.getPiece(squares::b8) == pieces::none &&
			board.getPiece(squares::c8) == pieces::none && board.getPiece(squares::d8) == pieces::none
			&& castleQAvailable && !isSquareReachableByWhite(board, squares::d8, lookup) &&
			!isSquareReachableByWhite(board, squares::e8, lookup) &&
			!lookup.includeOnlyCapturesAndPromotions)
		{
			assert(board.getPiece(squares::a8) == pieces::bR);
			Move move = createRegularSilentMove(board, sq, squares::c8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			addBlackIfValid(board, move, moves, lookup);
		}
	}

	void addWhitePawnMoves(BoardState& board, Square sq, const Lookup& lookup,
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
				addWhitePawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves, lookup);
			}
			else if (!lookup.includeOnlyCapturesAndPromotions)
			{
				addWhiteIfValid(board, createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, lookup);
			}
				

			// Double pawn advance.
			const Square doubleAdvanceSq = sq + 16;
			if (rank == 1 && board.getPiece(doubleAdvanceSq) == pieces::none
				&& !lookup.includeOnlyCapturesAndPromotions)
			{
				Move move = createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				addWhiteIfValid(board, move, moves, lookup);
			}
		}

		// Pawn captures.
		for (const Square captureSq : lookup.fSqL.getWhitePawnCaptureSquares()[sq])
		{
			const Piece other = board.getPiece(captureSq);
			if (EngineUtilities::isBlack(other))
			{
				if (rank == 6)
				{
					addWhitePawnCaptureWithPromotions(board, sq, captureSq, moves, lookup);
				}
				else
				{
					addWhiteIfValid(board,
						createWhiteRegularCapturingMove(board, sq, captureSq), moves, lookup);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq - 8) == pieces::bP);
				addWhiteIfValid(board, Move(pieces::wP, sq,
					captureSq, captureSq, pieces::bP, captureSq - 8), moves, lookup);
			}
		}
	}

	void addBlackPawnMoves(BoardState& board, Square sq, const Lookup& lookup,
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
				addBlackPawnAdvanceWithPromotions(board, sq, singleAdvanceSq, moves, lookup);
			}
			else if(!lookup.includeOnlyCapturesAndPromotions)
			{
				addBlackIfValid(board, createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, lookup);
			}


			// Double pawn advance.
			const Square doubleAdvanceSq = sq - 16;
			if (rank == 6 && board.getPiece(doubleAdvanceSq) == pieces::none
				&& !lookup.includeOnlyCapturesAndPromotions)
			{
				Move move = createRegularSilentMove(board, sq, doubleAdvanceSq);
				move.enPassantCreatedSquare = singleAdvanceSq; // One square behind the pawn.
				addBlackIfValid(board, move, moves, lookup);
			}
		}

		// Pawn captures.
		for (const Square captureSq : lookup.fSqL.getBlackPawnCaptureSquares()[sq])
		{
			const Piece other = board.getPiece(captureSq);
			if (EngineUtilities::isWhite(other))
			{
				if (rank == 1)
				{
					addBlackPawnCaptureWithPromotions(board, sq, captureSq, moves, lookup);
				}
				else
				{
					addBlackIfValid(board,
						createBlackRegularCapturingMove(board, sq, captureSq), moves, lookup);
				}
			}
			else if (captureSq == board.getEnPassantSquare())
			{
				assert(board.getPiece(captureSq + 8) == pieces::wP);
				addBlackIfValid(board, Move(pieces::bP, sq, captureSq,
					captureSq, pieces::wP, captureSq + 8), moves, lookup);
			}
		}
	}

	void addWhiteBishopMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH8(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA8(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH1(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA1(), moves, lookup);
	}

	void addBlackBishopMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH8(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA8(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH1(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA1(), moves, lookup);
	}

	void addWhiteKnightMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addWhiteRegularNonSweepMoves(board, sq, lookup.fSqL.getknightReachableSquares(),
			moves, lookup);
	}

	void addBlackKnightMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addBlackRegularNonSweepMoves(board, sq, lookup.fSqL.getknightReachableSquares(),
			moves, lookup);
	}

	void addWhiteRookMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addWhiteRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMin(), moves, lookup);
		addWhiteRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMax(), moves, lookup);
		addWhiteRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMin(), moves, lookup);
		addWhiteRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMax(), moves, lookup);
	}

	void addBlackRookMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addBlackRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMin(), moves, lookup);
		addBlackRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMax(), moves, lookup);
		addBlackRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMin(), moves, lookup);
		addBlackRookSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMax(), moves, lookup);
	}

	void addWhiteQueenMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH8(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA8(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH1(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA1(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMin(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMax(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMin(), moves, lookup);
		addWhiteRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMax(), moves, lookup);
	}

	void addBlackQueenMoves(BoardState& board, Square sq, const Lookup& lookup,
		std::vector<Move>& moves)
	{
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH8(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA8(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsH1(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getDiagTowardsA1(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMin(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsRankMax(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMin(), moves, lookup);
		addBlackRegularSweepMoves(board, sq, lookup.fSqL.getstraightTowardsFileMax(), moves, lookup);
	}

	std::vector<Move> getLegalWhiteMoves(BoardState& board, const FastSqLookup& fastSqLookup,
		bool onlyCapturesAndPromotions = false)
	{
		static constexpr size_t probableMax = 80;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		Lookup lookup(fastSqLookup);
		lookup.movingSideInCheckPreMove = isWhiteInCheck(board, lookup);
		lookup.includeOnlyCapturesAndPromotions = onlyCapturesAndPromotions;

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
			assert(EngineUtilities::isNonNoneSquare(sq));

			switch (board.getPiece(sq))
			{ 
				case pieces::wP:
					addWhitePawnMoves(board, sq, lookup, moves);
					break;
				case pieces::wN:
					addWhiteKnightMoves(board, sq, lookup, moves);
					break;
				case pieces::wB:
					addWhiteBishopMoves(board, sq, lookup, moves);
					break;
				case pieces::wR:
					addWhiteRookMoves(board, sq, lookup, moves);
					break;
				case pieces::wQ:
					addWhiteQueenMoves(board, sq, lookup, moves);
					break;
				case pieces::wK:
					addWhiteKingMoves(board, sq, lookup, moves);
					break;
			}
		}

		return moves;
	}

	std::vector<Move> getLegalBlackMoves(BoardState& board, const FastSqLookup& fastSqLookup,
		bool onlyCapturesAndPromotions = false)
	{
		static constexpr size_t probableMax = 80;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		Lookup lookup(fastSqLookup);
		lookup.movingSideInCheckPreMove = isBlackInCheck(board, lookup);
		lookup.includeOnlyCapturesAndPromotions = onlyCapturesAndPromotions;

		for (Square sq = squares::a1; sq < squares::num; sq++)
		{
			assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
			assert(EngineUtilities::isNonNoneSquare(sq));

			switch (board.getPiece(sq))
			{
				case pieces::bP:
					addBlackPawnMoves(board, sq, lookup, moves);
					break;
				case pieces::bN:
					addBlackKnightMoves(board, sq, lookup, moves);
					break;
				case pieces::bB:
					addBlackBishopMoves(board, sq, lookup, moves);
					break;
				case pieces::bR:
					addBlackRookMoves(board, sq, lookup, moves);
					break;
				case pieces::bQ:
					addBlackQueenMoves(board, sq, lookup, moves);
					break;
				case pieces::bK:
					addBlackKingMoves(board, sq, lookup, moves);
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


namespace moveCountHelpers
{
	size_t countMovesRecursive(const Engine& engine, BoardState& board, int32_t depth)
	{
		if (depth == 1)
		{
			return engine.getLegalMoves(board).size();
		}

		const auto moves = engine.getLegalMoves(board);
		size_t num = 0;
		for (const auto& move : moves)
		{
#ifndef NDEBUG
			const BoardState boardPreMove = board;
#endif

			assert(board.isValid());
			board.makeMove(move);
			assert(board.isValid());
			num += countMovesRecursive(engine, board, depth - 1);
			assert(board.isValid());
			board.unmakeMove(move);
			assert(board.isValid());

			assert(boardPreMove.getPieces() == board.getPieces());
			assert(boardPreMove.getCastleAvailability() == board.getCastleAvailability());
			assert(boardPreMove.getEnPassantSquare() == board.getEnPassantSquare());
			assert(boardPreMove.getTurn() == board.getTurn());
		}
		return num;
	}
}

std::optional<size_t> Engine::getNumLegalMoves(const std::string& FEN, int32_t depth) const
{
	if (depth <= 0)
	{
		EngineUtilities::logE("getNumLegalMoves failed, depth must be at least 1.");
		return {};
	}

	BoardState board;
	if (!board.initFromFEN(FEN))
	{
		return {};
	}

	return moveCountHelpers::countMovesRecursive(*this, board, depth);
}

hceEngine::StaticEvaluationResult Engine::evaluateStatic(const std::string& FEN) const
{
	BoardState board;
	if (!board.initFromFEN(FEN))
	{
		EngineUtilities::logE("evaluateStatic failed, invalid FEN.");
		return hceEngine::StaticEvaluationResult::Invalid;
	}

	const int32_t score = boardEvaluator.getScore(board, fastSqLookup);
	if (board.getTurn() == pieces::Color::WHITE)
	{
		if (score < 0) { return hceEngine::StaticEvaluationResult::BlackBetter; }
		else if (score > 0) { return hceEngine::StaticEvaluationResult::WhiteBetter; }
	}
	else
	{
		if (score < 0) { return hceEngine::StaticEvaluationResult::BlackBetter; }
		else if (score > 0) { return hceEngine::StaticEvaluationResult::WhiteBetter; }
	}

	return hceEngine::StaticEvaluationResult::Equal;
}

hceEngine::SearchResult Engine::getBestMove(const std::string& FEN, int32_t depth) const
{
	if (depth <= 0)
	{
		EngineUtilities::logE("getBestMove failed, depth must be at least 1.");
		return hceEngine::SearchResult(); // TODO: add invalid flag to the ChessMove.
	}

	BoardState board;
	if (!board.initFromFEN(FEN))
	{
		EngineUtilities::logE("getBestMove failed, invalid FEN.");
		return hceEngine::SearchResult(); // TODO: add invalid flag to the ChessMove.
	}

	Move bestMove;
	int32_t bestScore = searchHelpers::minusInf;
	for (const Move& m : getLegalMoves(board))
	{
		board.makeMove(m);
		int32_t score = -alphaBeta(board, 
			-1000000, 1000000, depth - 1);
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = m;
		}

		board.unmakeMove(m);
	}

	return hceEngine::SearchResult(); // TODO fill with data.
}

hceEngine::SearchResult Engine::getBestMoveMiniMax(const std::string& FEN, int32_t depth) const
{
	if (depth <= 0)
	{
		EngineUtilities::logE("getBestMoveMiniMax failed, depth must be at least 1.");
		return hceEngine::SearchResult(); // TODO: add invalid flag to the ChessMove.
	}

	BoardState board;
	if (!board.initFromFEN(FEN))
	{
		EngineUtilities::logE("getBestMoveMiniMax failed, invalid FEN.");
		return hceEngine::SearchResult(); // TODO: add invalid flag to the ChessMove.
	}

	searchHelpers::SearchInfo info;
	Move bestMove;
	int32_t bestScore = searchHelpers::minusInf;
	for (const Move& m : getLegalMoves(board))
	{
		board.makeMove(m);
		int32_t score = -negaMax(board, depth - 1, info);
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = m;
		}

		board.unmakeMove(m);
	}

	hceEngine::SearchResult result;
	result.engineInfo.depthsCompletelyCovered = depth;
	result.engineInfo.maxDepthVisited = depth; // No quiescence search.
	result.engineInfo.leafNodesVisited = info.leafNodesEvaluated;
	result.engineInfo.positionEvaluation = bestScore;
	return result;
}

std::vector<Move> Engine::getCaptureAndPromotionMoves(BoardState& board) const
{
	if (board.getTurn() == pieces::Color::WHITE)
	{
		return moveGenerationHelpers::getLegalWhiteMoves(board, fastSqLookup, true);
	}
	else
	{
		return moveGenerationHelpers::getLegalBlackMoves(board, fastSqLookup, true);
	}
}

int32_t Engine::negaMax(BoardState& board, int32_t depth, searchHelpers::SearchInfo& info) const
{
	if (depth <= 0)
	{
		info.leafNodesEvaluated++;
		return boardEvaluator.getScore(board, fastSqLookup);
	}

	int32_t bestScore = searchHelpers::minusInf;
	for (const Move& move : getLegalMoves(board))
	{
		board.makeMove(move);
		int32_t score = -negaMax(board, depth - 1, info);
		board.unmakeMove(move);
		if (score > bestScore)
		{
			bestScore = score;
		}
	}

	return bestScore;
}

int32_t Engine::alphaBeta(BoardState& board, int32_t alpha, int32_t beta, int32_t depth) const
{
	if (depth <= 0)
	{
		return boardEvaluator.getScore(board, fastSqLookup);
	}

	for (const Move& move : getLegalMoves(board))
	{
		board.makeMove(move);
		int32_t score = -alphaBeta(board, -beta, -alpha, depth - 1);
		board.unmakeMove(move);
		if (score >= beta)
		{
			// Prune.
			return beta;
		}

		if (score > alpha)
		{
			alpha = score;
		}
	}

	return alpha;
}
