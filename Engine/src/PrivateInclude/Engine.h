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

	// Includes moves that causes moving side to in check after the move, i.e. pseudo-legal.
	std::vector<Move> getPseudoLegalMoves(BoardState& board) const;
	std::vector<Move> getPseudoLegalCaptureAndPromotionMoves(BoardState& board) const;

	// Convenient tester-functions that checks that the pseudo-legal move generation is correct.
	// Only to be used in testing situations, e.g. in debug builds.
	bool dbgTestPseudoLegalMoveGeneration(BoardState& board,
		const std::vector<Move>& pseudoLegalMoves, bool wasInCheckPreMove) const;
	bool dbgTestPseudoLegalCaptAndPromotMoveGeneration(BoardState& board,
		const std::vector<Move>& pseudoLegalMoves, bool wasInCheckPreMove) const;

	
	int32_t negaMax(BoardState& board, int32_t depth, searchHelpers::SearchInfo& info) const;
	
	int32_t alphaBeta(BoardState& board, int32_t alpha, int32_t beta, int32_t depth,
		int32_t staticEval, searchHelpers::SearchInfo& info) const;
	
	int32_t alphaBetaQuiescence(BoardState& board, int32_t alpha, int32_t beta, int32_t currDepth,
		int32_t staticEval, searchHelpers::SearchInfo& info) const;
	
	void setStaticEvalAndSortMoves(BoardState& board, std::vector<Move>& moves) const;
	
	// Tries to use the fast evaluation delta scheme offered by the BoardEvaluator. A valid pre-move
	// static evaluation score must be provided to use this function!
	void setStaticEvalUsingDeltaAndSortMoves(BoardState& board, std::vector<Move>& moves,
		int32_t staticEval) const;

	FastSqLookup fastSqLookup;
	BoardEvaluator boardEvaluator;
};
