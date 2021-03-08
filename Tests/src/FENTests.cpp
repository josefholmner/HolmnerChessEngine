#include "FENTests.h"

#include "Utilities.h"

#include "Engine/EngineAPI.h"

#include <iostream>

void FENTests::Run()
{
	hceEngine::EngineAPI engine;
	const auto numMoves = engine.getNumLegalMoves(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1);

	if (!numMoves)
	{
		Utilities::LogE("FEN Tests failed.");
	}
	else
	{
		Utilities::Log("FEN Tests succeeded.");
	}
}
