#pragma once

#include "PiecesAndSquares.h"
#include "BoardState.h"
#include "FastSqLookup.h"
#include "Move.h"
#include "SearchHelpers.h"

#include <cstdint>
#include <array>

/**
* The BoardEvaluator enables fast evaluation of a BoardState. Note that the construction of this
* object is slow, but once it has been constructed the evaluations are very fast. This means that
* instances of this class should be kept and stored somewhere for repeated use if possible.
*/
class BoardEvaluator
{
public:
	static Score getStaticEvaluation(const BoardState& board, const FastSqLookup& lookup);
	
	struct PreMoveInfo
	{
		std::array<bool, files::num> movingSidePawnsFileOccupation = { false };
	};

	// For some moves (NOT all!), the change in board static evaluation score that the move will cause
	// can be calculated quickly without having to make the move. IMPORTANT: always call the
	// canUseGetStaticEvaluationDelta() to determine if this function can be used or not for the move. 
	// If not, then the normal getStaticEvaluation() must be used.
	// If canUseGetStaticEvaluationDelta() returns true, it is guaranteed that these two options of
	// getting the static evaluations yiedls the same result but the latter is much faster;
	//
	// -OPTION 1-
	// makeMove()
	// eval = -getStaticEvaluation()
	// unmakeMove()
	//
	// -OPTION 2-
	// eval = preMoveEval + getStaticEvaluationDelta()
	//
	static Score getStaticEvaluationDelta(const BoardState& board, const Move& move,
		const PreMoveInfo& preMoveInfo, const FastSqLookup& lookup);
	static bool canUseGetStaticEvaluationDelta(const Move& move);

	static PreMoveInfo createPreMoveInfo(const BoardState& board);

	static Score getPawnBaseValue();
};
