#pragma once

#include "Engine/EngineAPIReturnDefinitions.h"

#include "FastSqLookup.h"
#include "BoardEvaluator.h"
#include "BoardState.h"

#include <vector>
#include <optional>

class Move;

class Engine
{
public:
	std::vector<Move> getLegalMoves(BoardState& board) const;

	std::optional<size_t> getNumLegalMoves(const std::string& FEN, int32_t depth) const;

	hceEngine::StaticEvaluationResult evaluateStatic(const std::string& FEN) const;

private:
	FastSqLookup fastSqLookup;
	BoardEvaluator boardEvaluator;
};
