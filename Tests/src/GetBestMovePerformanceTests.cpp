#include "GetBestMovePerformanceTests.h"

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
		TestsUtilities::log(positionName + 
			" analyzed using alpha-beta with quiescence search.\nCompletely covered depths: " +
			std::to_string(result.engineInfo.depthsCompletelyCovered) + " (max depth: " +
			std::to_string(result.engineInfo.maxDepthVisited) + ") took: " +
			std::to_string(milliseconds) + "ms.\nNumber of nodes visited: " +
			std::to_string(nodes) +
			+" which is: " + std::to_string((nodes / std::max(milliseconds, 1)) * 1000) + " nodes "
			+ "visited per second.\n");
	}

	void testMidGameAnalysisPerformance(const hceEngine::EngineAPI& engine, int32_t depth)
	{
		// Test starting position (fast) alpha-beta with quiescence search performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string midgamePos = "r3k2r/pppqbppp/2npbn2/4p3/4P3/2NPBN2/PPPQBPPP/R3K2R w KQkq - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMove(midgamePos, depth);
		const int32_t midgameTime = stopwatch.getMilliseconds();
		printResut("Mid-game position", res, midgameTime);
	}

	void testStartPosAnalysisPerformance(const hceEngine::EngineAPI& engine, int32_t depth)
	{
		// Test starting position (fast) alpha-beta with quiescence search performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMove(startPos, depth);
		const int32_t startPosTime = stopwatch.getMilliseconds();
		printResut("Starting position", res, startPosTime);
	}
}

void GetBestMovePerformanceTests::Run()
{
	TestsUtilities::log("***** GET BEST MOVE PERFORMANCE TESTS START *****");
	hceEngine::EngineAPI engine;

	static constexpr int32_t startPosDepth = 7;
	static constexpr int32_t midgameDepth = 7;
	testStartPosAnalysisPerformance(engine, startPosDepth);
	testMidGameAnalysisPerformance(engine, midgameDepth);

	TestsUtilities::log("All get best move performance tests done.");
	TestsUtilities::log("***** GET BEST MOVE PERFORMANCE TESTS END *****\n\n");
}
