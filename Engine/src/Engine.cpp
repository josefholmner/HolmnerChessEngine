#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Board.h"
#include "PrivateInclude/EngineUtilities.h"

#include <cassert>

namespace Engine_helpers
{
	// @todo compare performance using std::vector.
	struct MoveCollection
	{
		SmallVal num = 0;
		Move moves[MAX_NUM_MOVES];
	};

	void getPseudoLegalPawnMovesWhite(const Board& board, Square square, MoveCollection& outMoves)
	{

	}

	void getPseudoLegalPawnMovesBlack(const Board& board, Square square, MoveCollection& outMoves)
	{

	}

	void getPseudoLegalPawnMoves(const Board& board, Square square, MoveCollection& outMoves)
	{
		if (board.getSideToPlay() == WHITE)
			getPseudoLegalPawnMovesWhite(board, square, outMoves);
		else
			getPseudoLegalPawnMovesBlack(board, square, outMoves);
	}

	MoveCollection getPseudoLegalMoves(const Board& board)
	{
		MoveCollection moves;
		return moves;
	}


	size_t countMovesRecursive(Board& board, SmallVal depth)
	{
		if (depth == 1)
		{
			// @todo look for checks!
			return getPseudoLegalMoves(board).num;
		}

		const auto moves = getPseudoLegalMoves(board);
		size_t num = 0;
		for (int i = 0; i < moves.num; i++)
		{
			const Move move = moves.moves[i];
#ifndef NDEBUG
			const Board boardPreMove = board;
#endif

			board.makeMove(move);
			assert(board.isValid());

			num += countMovesRecursive(board, depth - 1);
			assert(board.isValid());

			board.unMakeMove(move);
			assert(board.isValid());
			assert(board == boardPreMove);
		}

		return num;
	}
}

std::optional<size_t> Engine::getNumLegalMoves(const std::string& FEN, uint8_t depth) const
{
  if (depth == 0)
  {
    EngineUtilities::logE("getNumLegalMoves failed, depth must be at least 1.");
    return {};
  }

  Board board;
  if (!board.init(FEN))
    return {};

	return Engine_helpers::countMovesRecursive(board, depth);
}
