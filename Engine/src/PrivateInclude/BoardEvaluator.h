#pragma once

#include "PiecesAndSquares.h"

#include <cstdint>
#include <array>

class BoardState;
class FastSqLookup;

/**
* The BoardEvaluator enables fast evaluation of a BoardState. Note that the construction of this
* object is slow, but once it has been constructed the evaluations are very fast. This means that
* instances of this class should be kept and stored somewhere for repeated use if possible.
*/
class BoardEvaluator
{
public:
	BoardEvaluator();

	int32_t getScore(const BoardState& board, const FastSqLookup& lookup) const;

private:
	void init();

	int32_t getWhiteKingScore(const BoardState& board, int32_t numBlackMajorPieces) const;
	int32_t getBlackKingScore(const BoardState& board, int32_t numWhiteMajorPieces) const;

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
