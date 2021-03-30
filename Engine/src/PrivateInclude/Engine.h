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

	std::optional<size_t> getNumLegalMoves(const std::string& FEN, Depth depth) const;

	hceEngine::StaticEvaluationResult evaluateStatic(const std::string& FEN) const;

	hceEngine::SearchResult getBestMove(const std::string& FEN, Depth depth) const;

	hceEngine::SearchResult getBestMoveMiniMax(const std::string& FEN, Depth depth) const;

private:
	std::vector<Move> getCaptureAndPromotionMoves(BoardState& board) const;

	// Includes moves that causes moving side to in check after the move, i.e. pseudo-legal.
	std::vector<Move> getPseudoLegalMoves(BoardState& board) const;

	// Convenient tester-functions that checks that the pseudo-legal move generation is correct.
	// Only to be used in testing situations, e.g. in debug builds.
	bool dbgTestPseudoLegalMoveGeneration(BoardState& board,
		const std::vector<Move>& pseudoLegalMoves, bool wasInCheckPreMove) const;

	
	Score negaMax(BoardState& board, Depth depth, searchHelpers::SearchInfo& info) const;
	
	Score alphaBeta(BoardState& board, Score alpha, Score beta, Depth depth,
		Score staticEval, searchHelpers::SearchInfo& info) const;
	
	Score alphaBetaQuiescence(BoardState& board, Score alpha, Score beta, Depth currDepth,
		Score staticEval, searchHelpers::SearchInfo& info) const;
	
	void setStaticEvalAndSortMoves(BoardState& board, std::vector<Move>& moves) const;
	
	// Tries to use the fast evaluation delta scheme offered by the BoardEvaluator. A valid pre-move
	// static evaluation score must be provided to use this function!
	void setStaticEvalUsingDeltaAndSortMoves(BoardState& board, std::vector<Move>& moves,
		Score staticEval) const;

	FastSqLookup fastSqLookup;
	BoardEvaluator boardEvaluator;
};
