#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Move.h"
#include "PrivateInclude/EngineUtilities.h"

#include <cassert>
#include <algorithm>

namespace moveGenerationHelpers
{
	// Use of 'CaptAndPromot' is faster than 'All' since it is only a subset of the moves.
	enum class TypeFilter
	{
		All, // Include ALL moves, both silent and capturing.
		CaptAndPromot // Include only captures and pawn promotions.
	};

	// Use of 'PseudoLegal' is faster than 'StrictlyLegal' since no looking for moving-side check is done.
	enum class LegalityFilter
	{
		StrictlyLegal, // Include only legal moves (ommits moves causing moving-side check).
		PseudoLegal // Includes same moves as 'StrictlyLegal', plus those causing moving-side check.
	};

	struct Lookup
	{
		Lookup(const FastSqLookup& fastSqLookup) : fSqL{fastSqLookup}{}

		const FastSqLookup& fSqL;
		bool movingSideInCheckPreMove = true;
		TypeFilter tFilter = TypeFilter::All;
		LegalityFilter lFilter = LegalityFilter::StrictlyLegal;
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
		assert(board.getPiece(board.getWhiteKingSquare()) == pieces::wK);
		return isSquareReachableByBlack(board, board.getWhiteKingSquare(), fastSqLookup);
	}

	bool isBlackInCheck(const BoardState& board, const FastSqLookup& fastSqLookup)
	{
		assert(board.getPiece(board.getBlackKingSquare()) == pieces::bK);
		return isSquareReachableByWhite(board, board.getBlackKingSquare(), fastSqLookup);
	}

	bool doesWhiteMoveCauseMovingSideCheck(BoardState& board, const FastSqLookup& fastSqLookup,
		const Move& move, bool wasInCheckPreMove)
	{
		if (wasInCheckPreMove)
		{
			if (move.capturedPiece == pieces::none && !fastSqLookup.areSquaresInLinearLineOfSight(
				move.toSquare, board.getWhiteKingSquare()) && move.movingPiece != pieces::wK)
			{
				// Non-capturing, non-king move ending up not in line of sight of the king cannot
				// make us not in check, so this move will make the moving side still be in check.
				return true;
			}
		}
		else
		{
			if (!fastSqLookup.areSquaresInLinearLineOfSight(move.fromSquare, board.getWhiteKingSquare())
				&& move.movingPiece != pieces::wK && move.toSquare != board.getEnPassantSquare())
			{
				return false;
			}
		}

		// Above optimizations failed, temporarily make the move, and look for check.
		board.makeMove(move);
		const bool causesCheck = isWhiteInCheck(board, fastSqLookup);
		board.unmakeMove(move);
		return causesCheck;
	}

	bool doesBlackMoveCauseMovingSideCheck(BoardState& board, const FastSqLookup& fastSqLookup,
		const Move& move, bool wasInCheckPreMove)
	{
		if (wasInCheckPreMove)
		{
			if (move.capturedPiece == pieces::none && !fastSqLookup.areSquaresInLinearLineOfSight(
				move.toSquare, board.getBlackKingSquare()) && move.movingPiece != pieces::bK)
			{
				// Non-capturing, non-king move ending up not in line of sight of the king cannot
				// make us not in check, so this move will make the moving side still be in check.
				return true;
			}
		}
		else
		{
			if (!fastSqLookup.areSquaresInLinearLineOfSight(move.fromSquare, board.getBlackKingSquare())
				&& move.movingPiece != pieces::bK && move.toSquare != board.getEnPassantSquare())
			{
				return false;
			}
		}

		// Above optimizations failed, temporarily make the move, and look for check.
		board.makeMove(move);
		const bool causesCheck = isBlackInCheck(board, fastSqLookup);
		board.unmakeMove(move);
		return causesCheck;
	}

	bool doesMoveCauseMovingSideCheck(BoardState& board, const FastSqLookup& fastSqLookup,
		const Move& move, bool wasInCheckPreMove)
	{
		// Optimization: if the side whos turn it is was not in check before making the move, a
		// move that involves moving a piece that is not in line with its own king prior to making
		// the move cannot cause the moving side to move itself into check. This is true as long as
		// the moving piece is not the king itself or if making an en passant capture. 
		// Simularly, if the moving side was already in check before the move, any non-capturing
		// move with its end square not in line with its own king cannot make the moving side not
		// in check. We use these facts here to not having to do the move to determine if it would
		// cause a check or not.
		if (board.getTurn() == pieces::Color::WHITE)
		{
			return doesWhiteMoveCauseMovingSideCheck(board, fastSqLookup, move, wasInCheckPreMove);
		}
		else
		{
			return doesBlackMoveCauseMovingSideCheck(board, fastSqLookup, move, wasInCheckPreMove);
		}
	}

	bool isInCheck(BoardState& board, const FastSqLookup& fastSqLookup)
	{
		if (board.getTurn() == pieces::Color::WHITE)
		{
			return isWhiteInCheck(board, fastSqLookup);
		}
		else
		{
			return isBlackInCheck(board, fastSqLookup);
		}
	}

	// Ensures that the moving side is not in check prior to adding the move.
	void addWhiteIfValid(BoardState& board, const Move& move, std::vector<Move>& moves,
		const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::WHITE);
		if (lookup.lFilter == LegalityFilter::PseudoLegal)
		{
			// We don't need to look for check, just add and return.
			moves.push_back(move);
			return;
		}

#ifndef NDEBUG
		const BoardState boardPreMove = board;
#endif

		if (doesWhiteMoveCauseMovingSideCheck(board, lookup.fSqL, move, lookup.movingSideInCheckPreMove))
		{
			// Filter out any moves that causes moving-side check since that is an invalid move.
			return;
		}

		moves.push_back(move);
		assert(board == boardPreMove);
	}

	// Ensures that the moving side is not in check prior to adding the move.
	void addBlackIfValid(BoardState& board, const Move& move,
		std::vector<Move>& moves, const Lookup& lookup)
	{
		assert(board.getTurn() == pieces::Color::BLACK);
		if (lookup.lFilter == LegalityFilter::PseudoLegal)
		{
			// We don't need to look for check, just add and return.
			moves.push_back(move);
			return;
		}


#ifndef NDEBUG
		const BoardState boardPreMove = board;
#endif

		if (doesBlackMoveCauseMovingSideCheck(board, lookup.fSqL, move, lookup.movingSideInCheckPreMove))
		{
			// Filter out any moves that causes moving-side check since that is an invalid move.
			return;
		}

		moves.push_back(move);
		assert(board == boardPreMove);
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
				if (!(lookup.tFilter == TypeFilter::CaptAndPromot))
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
				if (!(lookup.tFilter == TypeFilter::CaptAndPromot))
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
			if (other == pieces::none && !(lookup.tFilter == TypeFilter::CaptAndPromot))
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
			if (other == pieces::none && !(lookup.tFilter == TypeFilter::CaptAndPromot))
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
				if (!(lookup.tFilter == TypeFilter::CaptAndPromot))
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
				if (!(lookup.tFilter == TypeFilter::CaptAndPromot))
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
		assert(ranks::toRank(fromSquare) == ranks::rank7);
		assert(ranks::toRank(toSquare) == ranks::rank8);

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
		assert(ranks::toRank(fromSquare) == ranks::rank2);
		assert(ranks::toRank(toSquare) == ranks::rank1);

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
		assert(ranks::toRank(fromSquare) == ranks::rank7);
		assert(ranks::toRank(toSquare) == ranks::rank8);
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
		assert(ranks::toRank(fromSquare) == ranks::rank2);
		assert(ranks::toRank(toSquare) == ranks::rank1);
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
			if (other == pieces::none && !(lookup.tFilter == TypeFilter::CaptAndPromot))
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
			!isSquareReachableByBlack(board, squares::f1, lookup.fSqL) &&
			!isSquareReachableByBlack(board, squares::e1, lookup.fSqL) &&
			!(lookup.tFilter == TypeFilter::CaptAndPromot))
		{
			assert(board.getPiece(squares::h1) == pieces::wR);
			Move move = createRegularSilentMove(board, sq, squares::g1);
			move.prohibitsWKcastling = castleKAvailable;
			move.prohibitsWQcastling = castleQAvailable;
			addWhiteIfValid(board, move, moves, lookup);
		}

		if (sq == squares::e1 && board.getPiece(squares::b1) == pieces::none &&
			board.getPiece(squares::c1) == pieces::none && board.getPiece(squares::d1) == pieces::none
			&& castleQAvailable && !isSquareReachableByBlack(board, squares::d1, lookup.fSqL)
			&& !isSquareReachableByBlack(board, squares::e1, lookup.fSqL)
			&& !(lookup.tFilter == TypeFilter::CaptAndPromot))
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
			if (other == pieces::none && !(lookup.tFilter == TypeFilter::CaptAndPromot))
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
			!isSquareReachableByWhite(board, squares::f8, lookup.fSqL) &&
			!isSquareReachableByWhite(board, squares::e8, lookup.fSqL) &&
			!(lookup.tFilter == TypeFilter::CaptAndPromot))
		{
			assert(board.getPiece(squares::h8) == pieces::bR);
			Move move = createRegularSilentMove(board, sq, squares::g8);
			move.prohibitsBKcastling = castleKAvailable;
			move.prohibitsBQcastling = castleQAvailable;
			addBlackIfValid(board, move, moves, lookup);
		}

		if (sq == squares::e8 && board.getPiece(squares::b8) == pieces::none &&
			board.getPiece(squares::c8) == pieces::none && board.getPiece(squares::d8) == pieces::none
			&& castleQAvailable && !isSquareReachableByWhite(board, squares::d8, lookup.fSqL) &&
			!isSquareReachableByWhite(board, squares::e8, lookup.fSqL) &&
			!(lookup.tFilter == TypeFilter::CaptAndPromot))
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
			else if (!(lookup.tFilter == TypeFilter::CaptAndPromot))
			{
				addWhiteIfValid(board, createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, lookup);
			}
				

			// Double pawn advance.
			const Square doubleAdvanceSq = sq + 16;
			if (rank == 1 && board.getPiece(doubleAdvanceSq) == pieces::none
				&& !(lookup.tFilter == TypeFilter::CaptAndPromot))
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
			else if(!(lookup.tFilter == TypeFilter::CaptAndPromot))
			{
				addBlackIfValid(board, createRegularSilentMove(
					board, sq, singleAdvanceSq), moves, lookup);
			}


			// Double pawn advance.
			const Square doubleAdvanceSq = sq - 16;
			if (rank == 6 && board.getPiece(doubleAdvanceSq) == pieces::none
				&& !(lookup.tFilter == TypeFilter::CaptAndPromot))
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
		const TypeFilter& tFilter = TypeFilter::All,
		const LegalityFilter& lFilter = LegalityFilter::StrictlyLegal)
	{
		static constexpr size_t probableMax = 80;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		Lookup lookup(fastSqLookup);
		lookup.movingSideInCheckPreMove = lFilter == LegalityFilter::StrictlyLegal ?
			isWhiteInCheck(board, lookup.fSqL) : false;
		lookup.tFilter = tFilter;
		lookup.lFilter = lFilter;

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
		const TypeFilter& tFilter = TypeFilter::All,
		const LegalityFilter& lFilter = LegalityFilter::StrictlyLegal)
	{
		static constexpr size_t probableMax = 80;
		std::vector<Move> moves;
		moves.reserve(probableMax);

		Lookup lookup(fastSqLookup);
		lookup.movingSideInCheckPreMove = lFilter == LegalityFilter::StrictlyLegal ?
			isBlackInCheck(board, lookup.fSqL) : false;
		lookup.tFilter = tFilter;
		lookup.lFilter = lFilter;

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
	size_t countMovesRecursive(const Engine& engine, BoardState& board, Depth depth)
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

			board.makeMove(move);
			assert(board.isValid());
			num += countMovesRecursive(engine, board, depth - 1);
			assert(board.isValid());
			board.unmakeMove(move);
			assert(board.isValid());

			assert(board == boardPreMove);
		}
		return num;
	}
}

std::optional<size_t> Engine::getNumLegalMoves(const std::string& FEN, Depth depth) const
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

	const Score score = boardEvaluator.getStaticEvaluation(board, fastSqLookup);
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

hceEngine::SearchResult Engine::getBestMove(const std::string& FEN, Depth depth) const
{
	hceEngine::SearchResult searchResult;

	if (depth <= 0)
	{
		EngineUtilities::logE("getBestMove failed, depth must be at least 1.");
		searchResult.bestMove.type = hceEngine::MoveType::Invalid;
		return searchResult;
	}

	BoardState board(true);
	if (!board.initFromFEN(FEN))
	{
		EngineUtilities::logE("getBestMove failed, invalid FEN.");
		hceEngine::SearchResult res;
		searchResult.bestMove.type = hceEngine::MoveType::Invalid;
		return searchResult;
	}

	searchHelpers::SearchInfo info;
	Move bestMove;
	searchHelpers::tp::MinimalMoveInfo bestMoveLastDepth;
	Score bestScore;
	auto moves = getLegalMoves(board);
	Depth currentDepth = 1;
	while (currentDepth <= depth)
	{
		Score alpha = searchHelpers::minusInf;
		static constexpr Score beta = searchHelpers::plusInf;
		bestScore = searchHelpers::minusInf;
		if (bestMoveLastDepth.isSet())
		{
			setStaticEvalAndSortMoves(board, moves, bestMoveLastDepth);
		}
		else
		{
			setStaticEvalAndSortMoves(board, moves);
		}
		
		for (const Move& m : moves)
		{
			board.makeMove(m);
			info.nodesVisited++;
			const int32_t score = -alphaBeta(
				board, -beta, -alpha, currentDepth-1, m.staticEval, info);
			if (score > bestScore)
			{
				bestScore = score;
				bestMove = m;
				bestMoveLastDepth.from = m.fromSquare;
				bestMoveLastDepth.to = m.toSquare;
			}

			if (score > alpha)
			{
				alpha = score;
			}

			board.unmakeMove(m);
		}

		currentDepth++;
	}

	searchResult.bestMove.fromSquare = squares::squareToStr(bestMove.fromSquare);
	searchResult.bestMove.toSquare = squares::squareToStr(bestMove.toSquare);
	searchResult.engineInfo.depthsCompletelyCovered = depth;
	searchResult.engineInfo.maxDepthVisited = (size_t)depth + info.quiescenceMaxDepth;
	searchResult.engineInfo.nodesVisited = info.nodesVisited;
	searchResult.bestMove.positionEvaluation = bestScore;
	return searchResult;
}

hceEngine::SearchResult Engine::getBestMoveMiniMax(const std::string& FEN, Depth depth) const
{
	hceEngine::SearchResult searchResult;

	if (depth <= 0)
	{
		EngineUtilities::logE("getBestMoveMiniMax failed, depth must be at least 1.");
		searchResult.bestMove.type = hceEngine::MoveType::Invalid;
		return searchResult;
	}

	BoardState board;
	if (!board.initFromFEN(FEN))
	{
		EngineUtilities::logE("getBestMoveMiniMax failed, invalid FEN.");
		searchResult.bestMove.type = hceEngine::MoveType::Invalid;
		return searchResult;
	}

	searchHelpers::SearchInfo info;
	Move bestMove;
	Score bestScore = searchHelpers::minusInf;
	for (const Move& m : getLegalMoves(board))
	{
		board.makeMove(m);
		info.nodesVisited++;
		const Score score = -negaMax(board, depth - 1, info);
		if (score > bestScore)
		{
			bestScore = score;
			bestMove = m;
		}

		board.unmakeMove(m);
	}

	searchResult.bestMove.fromSquare = squares::squareToStr(bestMove.fromSquare);
	searchResult.bestMove.toSquare = squares::squareToStr(bestMove.toSquare);
	searchResult.engineInfo.depthsCompletelyCovered = depth;
	searchResult.engineInfo.maxDepthVisited = depth; // No quiescence search.
	searchResult.engineInfo.nodesVisited = info.nodesVisited;
	searchResult.bestMove.positionEvaluation = bestScore;
	return searchResult;
}

std::vector<Move> Engine::getCaptureAndPromotionMoves(BoardState& board) const
{
	using namespace moveGenerationHelpers;
	if (board.getTurn() == pieces::Color::WHITE)
	{
		return getLegalWhiteMoves(board, fastSqLookup, TypeFilter::CaptAndPromot);
	}
	else
	{
		return getLegalBlackMoves(board, fastSqLookup, TypeFilter::CaptAndPromot);
	}
}

std::vector<Move> Engine::getPseudoLegalMoves(BoardState& board) const
{
	using namespace moveGenerationHelpers;
	if (board.getTurn() == pieces::Color::WHITE)
	{
		return getLegalWhiteMoves(board, fastSqLookup, TypeFilter::All, LegalityFilter::PseudoLegal);
	}
	else
	{
		return getLegalBlackMoves(board, fastSqLookup, TypeFilter::All, LegalityFilter::PseudoLegal);
	}
}

bool Engine::dbgTestPseudoLegalMoveGeneration(BoardState& board,
	const std::vector<Move>& pseudoLegalMoves, bool wasInCheckPreMove) const
{
	int32_t numLegal = 0;
	for (const auto& pseudoLegalMove : pseudoLegalMoves)
	{
		if (!moveGenerationHelpers::doesMoveCauseMovingSideCheck(
			board, fastSqLookup, pseudoLegalMove, wasInCheckPreMove))
		{
			numLegal++;
		}
	}

	return numLegal == getLegalMoves(board).size();
}

Score Engine::negaMax(BoardState& board, Depth depth, searchHelpers::SearchInfo& info) const
{
	if (depth <= 0)
	{
		return boardEvaluator.getStaticEvaluation(board, fastSqLookup);
	}

	Score bestScore = searchHelpers::minusInf;
	for (const Move& move : getLegalMoves(board))
	{
		board.makeMove(move);
		info.nodesVisited++;
		const Score score = -negaMax(board, depth - 1, info);
		board.unmakeMove(move);
		if (score > bestScore)
		{
			bestScore = score;
		}
	}

	return bestScore;
}

Score Engine::alphaBeta(BoardState& board, Score alpha, Score beta, Depth depth,
	Score staticEval, searchHelpers::SearchInfo& info) const
{
	using namespace searchHelpers;
	using namespace moveGenerationHelpers;

	const Score alphaOrig = alpha;
	const tp::Element* elem = board.findTranspositionElement();
	if (elem != nullptr)
	{
		if (elem->depth >= depth)
		{
			switch (elem->type)
			{
				case tp::exact:
					return elem->score;
				case tp::lower:
					alpha = std::max(alpha, elem->score);
					break;
				case tp::upper:
					beta = std::min(beta, elem->score);
					break;
			}

			if (alpha >= beta)
			{
				return elem->score;
			}
		}
	}

	if (depth <= 0)
	{
		return alphaBetaQuiescence(board, alpha, beta, 0, staticEval, info);
	}

	Score bestScore = minusInf;
	tp::MinimalMoveInfo bestMove;
	auto moves = getPseudoLegalMoves(board);
	const bool inCheckPreMove = moves.size() > 0 ? isInCheck(board, fastSqLookup) : false;
	// assert(dbgTestPseudoLegalMoveGeneration(board, moves, inCheckPreMove)); /*Uncomment for testing*/
	if (elem != nullptr && elem->bestMove.isSet())
	{
		assert(elem->bestMove.to != squares::none);
		setStaticEvalUsingDeltaAndSortMoves(board, moves, staticEval, elem->bestMove);
	}
	else
	{
		setStaticEvalUsingDeltaAndSortMoves(board, moves, staticEval);
	}
	
	for (const Move& move : moves)
	{
		if (doesMoveCauseMovingSideCheck(board, fastSqLookup, move, inCheckPreMove))
		{
			// Filter out non-legal moves (thouse causing moveing side checks).
			continue;
		}

		board.makeMove(move);
		info.nodesVisited++;
		const Score score = -alphaBeta(
			board, -beta, -alpha, depth - 1, move.staticEval, info);
		board.unmakeMove(move);
		if (score > bestScore)
		{
			bestScore = score;
			bestMove.from = move.fromSquare;
			bestMove.to = move.toSquare;
		}

		alpha = std::max(alpha, score);
		if (alpha >= beta)
		{
			break;
		}
	}

	int8_t type;
	if (bestScore <= alphaOrig) type = tp::upper;
	else if (bestScore >= beta) type = tp::lower;
	else type = tp::exact;

	board.addTranspositionElement(tp::Element{bestScore, depth, type, bestMove});
	return bestScore;
}

Score Engine::alphaBetaQuiescence(BoardState& board, Score alpha, Score beta,
	Depth currDepth, Score staticEval, searchHelpers::SearchInfo& info) const
{
	using namespace moveGenerationHelpers;
	if (currDepth > info.quiescenceMaxDepth)
	{
		info.quiescenceMaxDepth = currDepth;
	}

	// Optimization: the staticEval was calculated one node above this during move sorting, so we
	// don't have to call the evaluation function here again.
	assert(staticEval == boardEvaluator.getStaticEvaluation(board, fastSqLookup));
	if (staticEval >= beta)
	{
		return beta;
	}

	if (alpha < staticEval)
	{
		alpha = staticEval;
	}

	auto moves = getCaptureAndPromotionMoves(board);
	setStaticEvalAndSortMoves(board, moves);
	for (const Move& move : moves)
	{
		assert(move.capturedPiece != pieces::none || move.pawnPromotionPiece != pieces::none);
		board.makeMove(move);
		info.nodesVisited++;
		const Score score = -alphaBetaQuiescence(board, -beta, -alpha, currDepth + 1,
			move.staticEval, info);
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

void Engine::setStaticEvalAndSortMoves(BoardState& board, std::vector<Move>& moves) const
{
	for (Move& move : moves)
	{
		board.makeMove(move);
		assert(board.isValid());
		move.staticEval = boardEvaluator.getStaticEvaluation(board, fastSqLookup);
		board.unmakeMove(move);
	}

	assert(board.isValid());
	std::sort(moves.begin(), moves.end());
}

void Engine::setStaticEvalAndSortMoves(BoardState& board, std::vector<Move>& moves, const searchHelpers::tp::MinimalMoveInfo& bestMove) const
{
	if (moves.size() <= 0)
	{
		return;
	}

	setStaticEvalAndSortMoves(board, moves);

	// Finally, we override the default sort and put the known bestMove at the beginning.
	setKnownBestMoveFirst(moves, bestMove);
}

void Engine::setKnownBestMoveFirst(std::vector<Move>& moves, const searchHelpers::tp::MinimalMoveInfo& bestMove) const
{
	assert(bestMove.isSet());

	for (int32_t i = 0; i < moves.size(); i++)
	{
		if (bestMove.from == moves[i].fromSquare && bestMove.to == moves[i].toSquare)
		{
			std::iter_swap(moves.begin(), moves.begin() + i);
			return;
		}
	}
}

void Engine::setStaticEvalUsingDeltaAndSortMoves(BoardState& board, std::vector<Move>& moves,
	Score staticEval) const
{
	if (moves.size() == 0)
	{
		return;
	}

	const auto preMoveInfo = boardEvaluator.createPreMoveInfo(board);
	for (Move& move : moves)
	{
		if (boardEvaluator.canUseGetStaticEvaluationDelta(move))
		{
			move.staticEval = -boardEvaluator.getStaticEvaluationDelta(
				board, move, preMoveInfo, fastSqLookup) - staticEval;

#ifndef NDEBUG
			board.makeMove(move);
			assert(move.staticEval == boardEvaluator.getStaticEvaluation(board, fastSqLookup));
			board.unmakeMove(move);
#endif
		}
		else
		{
			board.makeMove(move);
			assert(board.isValid());
			move.staticEval = boardEvaluator.getStaticEvaluation(board, fastSqLookup);
			board.unmakeMove(move);
		}
	}
	assert(board.isValid());
	std::sort(moves.begin(), moves.end());
}

void Engine::setStaticEvalUsingDeltaAndSortMoves(BoardState& board, std::vector<Move>& moves,
	Score staticEval, const searchHelpers::tp::MinimalMoveInfo& bestMove) const
{
	if (moves.size() == 0)
	{
		return;
	}

	// Do the regular sorting.
	setStaticEvalUsingDeltaAndSortMoves(board, moves, staticEval);

	// Finally, we override the default sort and put the known bestMove at the beginning.
	setKnownBestMoveFirst(moves, bestMove);
}
