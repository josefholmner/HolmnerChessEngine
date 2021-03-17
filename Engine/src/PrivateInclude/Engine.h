#pragma once

#include "FastSqLookup.h"

#include <vector>

class Move;
class BoardState;

class Engine
{
public:

	std::vector<Move> getLegalMoves(BoardState& board) const;

	const FastSqLookup& getFastSqLookup() const
	{
		return fastSqLookup;
	}

private:
	FastSqLookup fastSqLookup;
};
