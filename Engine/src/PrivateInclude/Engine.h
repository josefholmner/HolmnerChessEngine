#pragma once

#include <vector>

class Move;
class BoardState;

class Engine
{
public:

	std::vector<Move> getLegalMoves(BoardState& board) const;
};
