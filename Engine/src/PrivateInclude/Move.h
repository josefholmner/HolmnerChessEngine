#pragma once

#include "PiecesAndSquares.h"
#include "EngineUtilities.h"

#include <assert.h>

class Move
{
public:
	Move(Piece inMovingPiece, Square inFromSquare, Square inToSqaure) noexcept :
		movingPiece{ inMovingPiece },
		fromSquare{ inFromSquare },
		toSquare{ inToSqaure }
	{
		assert(EngineUtilities::isNonNonePiece(inMovingPiece));
		assert(EngineUtilities::isNonNoneSquare(inFromSquare));
		assert(EngineUtilities::isNonNoneSquare(inToSqaure));
	}
	
	Move(Piece inMovingPiece, Square inFromSquare, Square inToSqaure, Piece inCapturedPiece,
		Square inCapturedSquare) noexcept :
		movingPiece{inMovingPiece},
		fromSquare{inFromSquare},
		toSquare{inToSqaure},
		capturedPiece{ inCapturedPiece },
		capturedSquare { inCapturedSquare }
	{
		assert(EngineUtilities::isNonNonePiece(inMovingPiece));
		assert(EngineUtilities::isNonNoneSquare(inFromSquare));
		assert(EngineUtilities::isNonNoneSquare(inToSqaure));
		assert(EngineUtilities::isNonNonePiece(inCapturedPiece));
		assert(EngineUtilities::isNonNoneSquare(inCapturedSquare));
	}

	Piece movingPiece = pieces::none;	
	Square fromSquare = squares::none;
	Square toSquare = squares::none;
	Piece capturedPiece = pieces::none;
	Square capturedSquare = squares::none; // Will be same as toSquare except for en passant captures.

	// These are needed for pawn promotion or en passant caaptures/creation.
	Piece pawnPromotionPiece = pieces::none;
	Square enPassantCreatedSquare = squares::none;

	bool prohibitsWKcastling = false; // White king-side.
	bool prohibitsWQcastling = false; // White queen side.
	bool prohibitsBKcastling = false; // Black king side.
	bool prohibitsBQcastling = false; // Black queen side.
};
