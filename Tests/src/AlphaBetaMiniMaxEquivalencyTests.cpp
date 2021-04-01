#include "AlphaBetaMiniMaxEquivalencyTests.h"

#include "TestsUtilities.h"
#include "Engine/EngineAPI.h"

namespace
{
	using namespace hceEngine;

	bool isEqual(const SearchResult& res1, const SearchResult& res2)
	{
		return res1.bestMove.positionEvaluation == res2.bestMove.positionEvaluation;
	}
}

void AlphaBetaMiniMaxEquivalencyTests::Run()
{
	TestsUtilities::log("***** ALPHA BETA MINIMAX EQUIVALENCY TESTS START *****");

	hceEngine::EngineAPI engine;

	// Use the positions from the FEN tests list.
	const auto& FENtests = TestsUtilities::getFENTests();

	static const uint8_t maxDepth = TestsUtilities::isReleaseBuild() ? 6 : 3;
	for (size_t depth = 1; depth <= maxDepth; depth++)
	{
		for (size_t i = 0; i < FENtests.size(); i++)
		{
			const std::string test = FENtests[i];
			const TestsUtilities::FENTestParams params(test);

			// Since MiniMax does not do a quiescence search, we have to use the NoQuiescence
			// version of the alpha-beta getBestMove...(), otherwise they will not give the
			// same result.
			const auto& resultAlphaBeta = engine.getBestMoveNoQuiescence(params.FEN, depth);
			const auto& resultMiniMax = engine.getBestMoveMiniMax(params.FEN, depth);

			if (!isEqual(resultAlphaBeta, resultMiniMax))
			{
				TestsUtilities::logE("Alpha-beta MiniMax equivalency failed for test with index: " +
					std::to_string(i) + " depth: " + std::to_string(depth) + " FEN: " + params.FEN);
				return;
			}
		}

		TestsUtilities::log("Alpha-beta MiniMax equivalency depth: " + std::to_string(depth) +
			" succeeded.");
	}

	TestsUtilities::log("***** ALPHA BETA MINIMAX EQUIVALENCY TESTS END *****");
}
