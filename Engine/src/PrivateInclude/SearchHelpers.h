#pragma once

#include <cstdint>

typedef uint8_t Depth;
typedef int16_t Score;
typedef uint64_t Hash64;

namespace searchHelpers
{
	// Used because std::numeric_limits<int16_t>::min()/max() is not symmetrical.
	static constexpr Score plusInf = 10000;
	static constexpr Score minusInf = -plusInf;

	struct SearchInfo
	{
		int32_t nodesVisited = 0;
		int32_t quiescenceMaxDepth = 0;
	};

	namespace tp
	{
		static constexpr int8_t upper = 1;
		static constexpr int8_t lower = -1;
		static constexpr int8_t exact = 0;

		// A light-weight struct representing a Move.
		struct MoveID
		{
			Square from = squares::none;
			Square to = squares::none;
			bool isSet() const { return from != squares::none; }
		};

		struct Element
		{
			Score score;
			Depth depth;
			int8_t type;
			MoveID bestMove;
		};
	}
}
