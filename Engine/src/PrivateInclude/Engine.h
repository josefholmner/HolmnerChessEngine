#pragma once

#include "FastSqLookup.h"
#include "BoardState.h"

#include <vector>
#include <optional>

class Move;

class Engine
{
public:
	std::vector<Move> getLegalMoves(BoardState& board) const;

	std::optional<size_t> getNumLegalMoves(const std::string& FEN, int32_t depth) const;

private:
	FastSqLookup fastSqLookup;
};
