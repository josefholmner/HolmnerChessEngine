#include "FENTests.h"

#include "Utilities.h"

#include "EngineAPI.h"

#include <iostream>

void FENTests::Run()
{
	EngineAPI engine;
	const auto numMoves = engine.getNumLegalMoves(
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1);

	if (!numMoves)
	{
		Utilities::Log(Logger::Type::Error, "FEN Tests failed.");
	}
	else
	{
		Utilities::Log(Logger::Type::Log, "FEN Tests succeeded.");
	}
}
