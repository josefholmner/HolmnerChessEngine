#pragma once

#include "PiecesAndSquares.h"
#include "EngineUtilities.h"
#include "SearchHelpers.h"

#include <cassert>

class Move
{
public:
	Move() = default;

	Move(Piece inMovingPiece, Square inFromSquare, Square inToSqaure, Square inPreviousEnPassantSq) :
		movingPiece{ inMovingPiece },
		fromSquare{ inFromSquare },
		toSquare{ inToSqaure },
		previousEnPassantSquare{ inPreviousEnPassantSq }
	{
		assert(EngineUtilities::isNonNonePiece(inMovingPiece));
		assert(EngineUtilities::isNonNoneSquare(inFromSquare));
		assert(EngineUtilities::isNonNoneSquare(inToSqaure));
		assert(EngineUtilities::isValidSquare(inPreviousEnPassantSq));
	}
	
	Move(Piece inMovingPiece, Square inFromSquare, Square inToSqaure, Square inPreviousEnPassantSq,
		Piece inCapturedPiece, Square inCapturedSquare) :
		movingPiece{inMovingPiece},
		fromSquare{inFromSquare},
		toSquare{inToSqaure},
		previousEnPassantSquare{ inPreviousEnPassantSq },
		capturedPiece{ inCapturedPiece },
		capturedSquare { inCapturedSquare }
	{
		assert(EngineUtilities::isNonNonePiece(inMovingPiece));
		assert(EngineUtilities::isNonNoneSquare(inFromSquare));
		assert(EngineUtilities::isNonNoneSquare(inToSqaure));
		assert(EngineUtilities::isNonNonePiece(inCapturedPiece));
		assert(EngineUtilities::isNonNoneSquare(inCapturedSquare));
	}

	searchHelpers::tp::MoveID toMoveID() const
	{
		return searchHelpers::tp::MoveID{ fromSquare, toSquare };
	}

	bool operator < (const Move& other) const
	{
		return staticEval < other.staticEval;
	}

	Piece movingPiece = pieces::none;	
	Square fromSquare = squares::none;
	Square toSquare = squares::none;
	Piece capturedPiece = pieces::none;
	Square capturedSquare = squares::none; // Will be same as toSquare except for en passant captures.

	// These are needed for pawn promotion or en passant caaptures/creation.
	Piece pawnPromotionPiece = pieces::none;
	Square previousEnPassantSquare = squares::none;
	Square enPassantCreatedSquare = squares::none;

	bool prohibitsWKcastling = false; // White king-side.
	bool prohibitsWQcastling = false; // White queen side.
	bool prohibitsBKcastling = false; // Black king side.
	bool prohibitsBQcastling = false; // Black queen side.

	// The static evaluation of the board when this move has been made.
	Score staticEval = 0;
};
