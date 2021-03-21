#include "Engine/EngineAPI.h"

#include "PrivateInclude/Engine.h"

#include <cassert>

hceEngine::EngineAPI::EngineAPI()
{
    engine = std::make_unique<Engine>();
}

hceEngine::EngineAPI::~EngineAPI()
{
    // This destructor must be explicitly defined in this cpp file for the unique_ptr<Engine>
    // to work properly since Engine is forward delcared.
}

std::optional<size_t> hceEngine::EngineAPI::getNumLegalMoves(
    const std::string& FEN, int32_t depth) const
{
    assert(engine != nullptr);
    return engine->getNumLegalMoves(FEN, depth);
}

hceEngine::StaticEvaluationResult hceEngine::EngineAPI::evaluateStatic(const std::string& FEN) const
{
    assert(engine != nullptr);
    return engine->evaluateStatic(FEN);
}
