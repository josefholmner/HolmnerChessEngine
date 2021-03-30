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
			+ "visited per second. Best move score: " + std::to_string(result.bestMove.positionEvaluation)
			+ "\n");
	}

	void testEndGameAnalysisPerformance(const hceEngine::EngineAPI& engine, uint8_t depth)
	{
		// Test end-game position (fast) alpha-beta with quiescence search performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string endgamePos = "4k3/p1p1pp2/4b3/3p4/3P4/P7/1P2PP2/1N2K3 w - - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMove(endgamePos, depth);
		const int32_t endgameTime = stopwatch.getMilliseconds();
		printResut("End-game position", res, endgameTime);
	}

	void testMidGameAnalysisPerformance(const hceEngine::EngineAPI& engine, uint8_t depth)
	{
		// Test mid-game position (fast) alpha-beta with quiescence search performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string midgamePos = "r3k2r/pppqbppp/2npbn2/4p3/4P3/2NPBN2/PPPQBPPP/R3K2R w KQkq - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMove(midgamePos, depth);
		const int32_t midgameTime = stopwatch.getMilliseconds();
		printResut("Mid-game position", res, midgameTime);
	}

	void testLateMidGameAnalysisPerformance(const hceEngine::EngineAPI& engine, uint8_t depth)
	{
		// Test late mid-game position (fast) alpha-beta with quiescence search performance.
		hceCommon::Stopwatch stopwatch;
		static const std::string lateMidgamePos = "4r1k1/p4ppp/1rp1pnb1/3p4/P2P4/2QBP3/1PPN1PPP/6K1 w - - 0 1";
		stopwatch.start();
		const auto res = engine.getBestMove(lateMidgamePos, depth);
		const int32_t lateMidgameTime = stopwatch.getMilliseconds();
		printResut("Late mid-game position", res, lateMidgameTime);
	}

	void testStartPosAnalysisPerformance(const hceEngine::EngineAPI& engine, uint8_t depth)
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

	static const uint8_t startPosDepth = TestsUtilities::isReleaseBuild() ? 8 : 5;
	static const uint8_t midgameDepth = TestsUtilities::isReleaseBuild() ? 8 : 5;
	static const uint8_t lateMidgameDepth = TestsUtilities::isReleaseBuild() ? 9 : 5;
	static const uint8_t endgameDepth = TestsUtilities::isReleaseBuild() ? 10 : 6;
	testStartPosAnalysisPerformance(engine, startPosDepth);
	testMidGameAnalysisPerformance(engine, midgameDepth);
	testLateMidGameAnalysisPerformance(engine, lateMidgameDepth);
	testEndGameAnalysisPerformance(engine, endgameDepth);

	TestsUtilities::log("All get best move performance tests done.");
	TestsUtilities::log("***** GET BEST MOVE PERFORMANCE TESTS END *****\n\n");
}
