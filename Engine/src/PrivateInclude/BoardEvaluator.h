#pragma once

#include "PiecesAndSquares.h"
#include "PrivateInclude/BoardState.h"
#include "PrivateInclude/FastSqLookup.h"
#include "PrivateInclude/Move.h"

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
	BoardEvaluator();

	int32_t getStaticEvaluation(const BoardState& board, const FastSqLookup& lookup) const;
	
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
	int32_t getStaticEvaluationDelta(const BoardState& board, const Move& move,
		const FastSqLookup& lookup) const;
	bool canUseGetStaticEvaluationDelta(const Move& move) const;

private:
	void init();

	int32_t getWhiteKingScore(const BoardState& board, int32_t numBlackMajorPieces) const;
	int32_t getBlackKingScore(const BoardState& board, int32_t numWhiteMajorPieces) const;

	int32_t getPieceMoveQuickScore(const BoardState& board, const Move& move,
		const FastSqLookup& lookup) const;
	int32_t getWhiteBishopMoveQuickScore(const BoardState& board, const Move& move,
		const FastSqLookup& lookup) const;
	int32_t getBlackBishopMoveQuickScore(const BoardState& board, const Move& move,
		const FastSqLookup& lookup) const;

	std::array<int32_t, squares::num> wQueenStaticScores;
	std::array<int32_t, squares::num> bQueenStaticScores;

	std::array<int32_t, squares::num> wRookStaticScores;
	std::array<int32_t, squares::num> bRookStaticScores;

	std::array<int32_t, squares::num> wBishopStaticScores;
	std::array<int32_t, squares::num> bBishopStaticScores;

	std::array<int32_t, squares::num> wKnightStaticScores;
	std::array<int32_t, squares::num> bKnightStaticScores;

	std::array<int32_t, squares::num> wPawnStaticScores;
	std::array<int32_t, squares::num> bPawnStaticScores;

	std::array<int32_t, squares::num> wKingEndgameStaticScores;
	std::array<int32_t, squares::num> bKingEndgameStaticScores;
};
