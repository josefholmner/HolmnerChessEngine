#pragma once

#include <chrono>

namespace hceCommon
{
	class Stopwatch
	{
	public:
		void start();
		int32_t getMilliseconds();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	};
}

