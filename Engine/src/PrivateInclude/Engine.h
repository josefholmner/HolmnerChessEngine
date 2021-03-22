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

	hceEngine::BestMove getBestMoveMiniMax(const std::string& FEN, int32_t depth) const;

private:
	int32_t negaMax(BoardState& board, int32_t depth) const;

	FastSqLookup fastSqLookup;
	BoardEvaluator boardEvaluator;
};
