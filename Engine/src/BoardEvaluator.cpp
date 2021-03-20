#include "PrivateInclude/BoardEvaluator.h"

#include "PrivateInclude/EngineUtilities.h"
#include "PrivateInclude/BoardState.h"

#include <cassert>

int32_t BoardEvaluator::getScore(const BoardState& board)
{
	int32_t score = 0;

	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
		assert(EngineUtilities::isNonNoneSquare(sq));

		switch (board.getPiece(sq))
		{
			case pieces::wP:

				break;
			case pieces::wN:

				break;
			case pieces::wB:

				break;
			case pieces::wR:

				break;
			case pieces::wQ:

				break;
			case pieces::wK:

				break;
			case pieces::bP:

				break;
			case pieces::bN:

				break;
			case pieces::bB:

				break;
			case pieces::bR:

				break;
			case pieces::bQ:

				break;
			case pieces::bK:

				break;
		}
	}

	return score;
}
