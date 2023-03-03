#include "PrivateInclude/Engine.h"

#include "PrivateInclude/Board.h"
#include "PrivateInclude/EngineUtilities.h"

#include <cassert>

namespace Engine_helpers
{
	size_t countMovesRecursive(const Engine& engine, Board& board, SmallVal depth)
	{
		if (depth == 1)
		{
			return engine.getLegalMoves(board).num;
		}

		const auto moves = engine.getLegalMoves(board);
		size_t num = 0;
		for (int i = 0; i < moves.num; i++)
		{
			const Move move = moves.moves[i];
#ifndef NDEBUG
			const Board boardPreMove = board;
#endif

			board.makeMove(move);
			assert(board.isValid());
			num += countMovesRecursive(engine, board, depth - 1);
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

	return Engine_helpers::countMovesRecursive(*this, board, depth);
}

MoveCollection Engine::getLegalMoves(const Board& board) const
{
	MoveCollection moves;
	return moves;
}
