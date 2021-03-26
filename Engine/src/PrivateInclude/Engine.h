#pragma once

#include "Engine/EngineAPIReturnDefinitions.h"

#include "FastSqLookup.h"
#include "BoardEvaluator.h"
#include "BoardState.h"
#include "SearchHelpers.h"

#include <vector>
#include <optional>

class Move;

class Engine
{
public:
	std::vector<Move> getLegalMoves(BoardState& board) const;

	std::optional<size_t> getNumLegalMoves(const std::string& FEN, int32_t depth) const;

	hceEngine::StaticEvaluationResult evaluateStatic(const std::string& FEN) const;

	hceEngine::SearchResult getBestMove(const std::string& FEN, int32_t depth) const;

	hceEngine::SearchResult getBestMoveMiniMax(const std::string& FEN, int32_t depth) const;

private:
	std::vector<Move> getCaptureAndPromotionMoves(BoardState& board) const;
	
	int32_t negaMax(BoardState& board, int32_t depth, searchHelpers::SearchInfo& info) const;
	
	int32_t alphaBeta(BoardState& board, int32_t alpha, int32_t beta, int32_t depth,
		int32_t staticEval, searchHelpers::SearchInfo& info) const;
	
	int32_t alphaBetaQuiescence(BoardState& board, int32_t alpha, int32_t beta, int32_t currDepth,
		int32_t staticEval, searchHelpers::SearchInfo& info) const;
	
	void setStaticEvalAndSortMoves(BoardState& board, std::vector<Move>& moves) const;
	
	// Tries to use the fast score delta scheme offered by the BoardEvaluator. A valid static
	// evaluation score must be provided to use this function!
	void setStaticEvalUsingDeltaAndSortMoves(BoardState& board, std::vector<Move>& moves,
		int32_t staticEval) const;

	FastSqLookup fastSqLookup;
	BoardEvaluator boardEvaluator;
};
