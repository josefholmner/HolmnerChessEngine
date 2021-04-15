#pragma once

#include "Engine/EngineAPI.h"

#include <string>

class EngineWrapper
{
public:
	hceEngine::LegalMovesCollection getLegalMoves(const std::string& FEN);

private:
	hceEngine::EngineAPI engine;
};
