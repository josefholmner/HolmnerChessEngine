#include "RawMinimaxPerformanceTests.h"

#include "TestsUtilities.h"
#include "Engine/EngineAPI.h"
#include "Common/StopWatch.h"

void testMidGameAnalysisPerformance(const hceEngine::EngineAPI& engine, int32_t depth)
{
	// Test mid-game position (slow) minmax analysation performance.
	hceCommon::Stopwatch stopwatch;
	static const std::string midgamePos = "r3k2r/pppqbppp/2npbn2/4p3/4P3/2NPBN2/PPPQBPPP/R3K2R w KQkq - 0 1";
	stopwatch.start();
	const auto res = engine.getBestMoveMiniMax(midgamePos, depth);
	const int32_t midgameTime = stopwatch.getMilliseconds();
	TestsUtilities::log("Mid-game position to depth: " + std::to_string(depth) + " took: " +
		std::to_string(midgameTime) + "ms.");
}

void testStartPosAnalysisPerformance(const hceEngine::EngineAPI& engine, int32_t depth)
{
	// Test starting position (slow) minmax analysation performance.
	hceCommon::Stopwatch stopwatch;
	static const std::string startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	stopwatch.start();
	const auto res = engine.getBestMoveMiniMax(startPos, depth);
	const int32_t startPosTime = stopwatch.getMilliseconds();
	TestsUtilities::log("Starting position to depth: " + std::to_string(depth) + " took: " +
		std::to_string(startPosTime) + "ms.");
}

void RawMinimaxPerformanceTests::Run()
{
	hceEngine::EngineAPI engine;

	static constexpr int32_t startPosDepth = 6;
	static constexpr int32_t midgameDepth = 5;
	testStartPosAnalysisPerformance(engine, startPosDepth);
	testMidGameAnalysisPerformance(engine, midgameDepth);

	TestsUtilities::log("All raw minimax performance tests done.");
}
