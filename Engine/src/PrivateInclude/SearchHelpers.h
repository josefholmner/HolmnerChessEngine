#pragma once

#include <cstdint>

namespace searchHelpers
{
	// Used because std::numeric_limits<int32_t>::min()/max() is not symmetrical.
	static constexpr int32_t plusInf = 10000000;
	static constexpr int32_t minusInf = -plusInf;

	struct SearchInfo
	{
		size_t leafNodesEvaluated = 0;
		size_t quiescenceMaxDepth = 0;
	};
}