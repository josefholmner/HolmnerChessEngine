#include "RawMinimaxPerformanceTests.h"

#include "TestsUtilities.h"
#include "Engine/EngineAPI.h"
#include "Common/StopWatch.h"

#include <algorithm>

namespace
{
	void printResut(const std::string& positionName, const hceEngine::SearchResult& result,
		int32_t milliseconds)
	{
		const int32_t nodes = result.engineInfo.nodesVisited;
		TestsUtilities::log(positionName + " analyzed with minimax to to depth: " +
			std::to_string(result.engineInfo.depthsCompletelyCovered) + " took: " +
			std::to_string(milliseconds) + "ms.\nNumber of nodes visited: " +
			std::to_string(nodes) +
			+" which is: " + std::to_string((nodes / std::max(milliseconds, 1)) * 1000) + " nodes "
			+ "visited per second. Best move score: " + std::to_string(result.move.positionEvaluation)
			+ "\n");
	}

	void testMidGameAnalysisPerformance(const hceEngine::EngineAPI& engine, uint8_t depth)
	{
		// Test mid-game position (slow) minmax analysation performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string midgamePos = "r3k2r/pppqbppp/2npbn2/4p3/4P3/2NPBN2/PPPQBPPP/R3K2R w KQkq - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMoveMiniMax(midgamePos, depth);
		const int32_t midgameTime = stopwatch.getMilliseconds();
		printResut("Mid-game position", res, midgameTime);
	}

	void testStartPosAnalysisPerformance(const hceEngine::EngineAPI& engine, uint8_t depth)
	{
		// Test starting position (slow) minmax analysation performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMoveMiniMax(startPos, depth);
		const int32_t startPosTime = stopwatch.getMilliseconds();
		printResut("Starting position", res, startPosTime);
	}
}

void RawMinimaxPerformanceTests::Run()
{
	TestsUtilities::log("***** RAW MINIMAX PERFORMANCE TESTS START *****");
	hceEngine::EngineAPI engine;

	static const uint8_t startPosDepth = TestsUtilities::isReleaseBuild() ? 5 : 4;
	static const uint8_t midgameDepth = TestsUtilities::isReleaseBuild() ? 5 : 3;
	testStartPosAnalysisPerformance(engine, startPosDepth);
	testMidGameAnalysisPerformance(engine, midgameDepth);

	TestsUtilities::log("All raw minimax performance tests done.");
	TestsUtilities::log("***** RAW MINIMAX PERFORMANCE TESTS END *****\n\n");
}
