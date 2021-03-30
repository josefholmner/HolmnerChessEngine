#pragma once

#include <cstdint>

typedef uint8_t Depth;
typedef int16_t Score;

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
}
