#pragma once

#include <cstdint>

class BoardState;

/**
* The BoardEvaluator enables fast evaluation of a BoardState. Note that the construction of this
* object is slow, but once it has been constructed the evaluations are very fast. This means that
* instances of this class should be kept and stored somewhere for repeated use if possible.
*/
class BoardEvaluator
{
	int32_t getScore(const BoardState& board);
};
