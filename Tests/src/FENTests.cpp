#include "FENTests.h"

#include "TestsUtilities.h"
#include "Common/StopWatch.h"
#include "Engine/EngineAPI.h"


#include <string>

void FENTests::Run()
{
	TestsUtilities::log("***** FEN TESTS START *****");
	size_t numMovesSum = 0;
	hceEngine::EngineAPI engine;
	hceCommon::Stopwatch stopWatch;
	const auto& tests = TestsUtilities::getFENTests();
	stopWatch.start();

	static const uint8_t maxDepth = TestsUtilities::isReleaseBuild() ? 5 : 3;
	for (size_t depth = 1; depth <= maxDepth; depth++)
	{
		for (size_t i = 0; i < tests.size(); i++)
		{
			const std::string test = tests[i];
			const TestsUtilities::FENTestParams params(test);
			const auto numMoves = engine.getNumLegalMoves(params.FEN, depth);

			if (!numMoves)
			{
				TestsUtilities::logE("FEN test could not be run with test: " + test);
				return;
			}

			if (*numMoves != params.numMoves.find(depth)->second)
			{
				TestsUtilities::logE("FEN test failed with test number: " + std::to_string(i) +
					" depth: " + std::to_string(depth) + ".\nFEN: "
					+ test + "\nExpected num moves: " + std::to_string(params.numMoves.find(depth)->second)
					+ " but got: " + std::to_string(*numMoves));
				return;
			}

			numMovesSum += *numMoves;
		}

		TestsUtilities::log("FEN tests depth: " + std::to_string(depth) + " succeeded.");
	}

	int32_t mills = stopWatch.getMilliseconds();
	TestsUtilities::log("All FEN Tests finished successfully. Generated: " + std::to_string(numMovesSum)
		+ " moves in: " + std::to_string(mills) + " ms, or: " +
		std::to_string((numMovesSum / std::max(mills,1)) * 1000) + " moves per second.");

	TestsUtilities::log("***** FEN TESTS END *****\n\n");
}
