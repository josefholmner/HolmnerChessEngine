#include "Common/StopWatch.h"

void hceCommon::Stopwatch::start()
{
	startTime = std::chrono::high_resolution_clock::now();
}

int32_t hceCommon::Stopwatch::getMilliseconds()
{
	const auto stopTime = std::chrono::high_resolution_clock::now();
	return static_cast<int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
		stopTime - startTime).count());
}
