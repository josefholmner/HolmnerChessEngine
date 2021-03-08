#include "FENTests.h"

#include "TestsUtilities.h"

#include "Engine/EngineAPI.h"

#include <iostream>

void FENTests::Run()
{
	hceEngine::EngineAPI engine;
	const auto numMoves = engine.getNumLegalMoves(
		"r1bqkbnr/ppp1ppNp/3p4/8/4K3/3P4/PPPnPPPP/RNBQ1B1R w kq - 0 1", 1);

	if (!numMoves)
	{
		TestsUtilities::logE("FEN Tests failed.");
	}
	else
	{
		TestsUtilities::log("FEN Tests succeeded.");
	}
}
