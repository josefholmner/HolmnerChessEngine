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

	void testEndGameAnalysisPerformance(const hceEngine::EngineAPI& engine, int32_t depth)
	{
		// Test end-game position (fast) alpha-beta with quiescence search performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string endgamePos = "4k3/p1p1pp2/4b3/3p4/3P4/P7/1P2PP2/1N2K3 w - - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMove(endgamePos, depth);
		const int32_t endgameTime = stopwatch.getMilliseconds();
		printResut("End-game position", res, endgameTime);
	}

	void testMidGameAnalysisPerformance(const hceEngine::EngineAPI& engine, int32_t depth)
	{
		// Test mid-game position (fast) alpha-beta with quiescence search performance.
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

	static const int32_t startPosDepth = TestsUtilities::isReleaseBuild() ? 8 : 5;
	static const int32_t midgameDepth = TestsUtilities::isReleaseBuild() ? 7 : 4;
	static const int32_t endgameDepth = TestsUtilities::isReleaseBuild() ? 9 : 6;
	testStartPosAnalysisPerformance(engine, startPosDepth);
	testMidGameAnalysisPerformance(engine, midgameDepth);
	testEndGameAnalysisPerformance(engine, endgameDepth);

	TestsUtilities::log("All get best move performance tests done.");
	TestsUtilities::log("***** GET BEST MOVE PERFORMANCE TESTS END *****\n\n");
}
