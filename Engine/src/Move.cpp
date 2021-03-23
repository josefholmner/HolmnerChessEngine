#include "PrivateInclude/Move.h"

namespace
{
	int32_t scorePiece(Piece piece)
	{
		using namespace pieces;
		switch (piece)
		{
		case wP:
		case bP:
			return 1;
		case wB:
		case bB:
		case wN:
		case bN:
			return 3;
		case wR:
		case bR:
			return 5;
		case wQ:
		case bQ:
			return 9;
		case wK:
		case bK:
			return searchHelpers::plusInf;
		default:
			EngineUtilities::logE("Unknown piece given to getPieceVal.");
			return 0;
		}
	}

	int32_t scoreSquare(Square sq)
	{
		using namespace squares;

		switch (sq)
		{
		case d4:
		case d5:
		case e4:
		case e5:
			return 4;
		case c3:
		case c4:
		case c5:
		case c6:
		case d3:
		case d6:
		case e3:
		case e6:
		case f3:
		case f4:
		case f5:
		case f6:
			return 2;
		default:
			return 0;
		}
	}
}

int32_t Move::getQuickScore() const
{
	int32_t score = 0;

	// Give reward for moving towards center.
	score += scoreSquare(toSquare) - scoreSquare(fromSquare);

	// Reward capturing higher value pieces.
	if (capturedPiece != pieces::none)
	{
		score += scorePiece(capturedPiece) - scorePiece(movingPiece);
	}

	// Reward pawn promotion.
	if (pawnPromotionPiece != pieces::none)
	{
		score += scorePiece(pawnPromotionPiece) - scorePiece(movingPiece);
	}

	return score;
}
