#include "Engine/EngineAPI.h"

#include "PrivateInclude/Engine.h"

#include <cassert>

using namespace hceEngine;

EngineAPI::EngineAPI()
{
    engine = std::make_unique<Engine>();
}

EngineAPI::~EngineAPI()
{
    // This destructor must be explicitly defined in this cpp file for the unique_ptr<Engine>
    // to work properly since Engine is forward delcared.
}

std::optional<size_t> EngineAPI::getNumLegalMoves(
    const std::string& FEN, uint8_t depth) const
{
    assert(engine != nullptr);
    return engine->getNumLegalMoves(FEN, depth);
}

SearchResult hceEngine::EngineAPI::getBestMove(const std::string& FEN, uint8_t depth,
    int32_t timeoutMilliSeconds) const
{
    assert(engine != nullptr);
    return engine->getBestMove(FEN, depth, timeoutMilliSeconds);
}

SearchResult EngineAPI::getBestMoveMiniMax(const std::string& FEN, uint8_t depth) const
{
    assert(engine != nullptr);
    return engine->getBestMoveMiniMax(FEN, depth);
}

SearchResult hceEngine::EngineAPI::getWorstMoveMiniMax(const std::string& FEN, uint8_t depth) const
{
    assert(engine != nullptr);
    return engine->getWorstMoveMiniMax(FEN, depth);
}

LegalMovesCollection hceEngine::EngineAPI::getLegalMoves(const std::string& FEN) const
{
    assert(engine != nullptr);
    return engine->getLegalMoves(FEN);
}

StaticEvaluationResult EngineAPI::evaluateStatic(const std::string& FEN) const
{
    assert(engine != nullptr);
    return engine->evaluateStatic(FEN);
}
