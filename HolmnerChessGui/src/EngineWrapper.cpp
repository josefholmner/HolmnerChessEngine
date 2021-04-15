#include "EngineWrapper.h"

hceEngine::LegalMovesCollection EngineWrapper::getLegalMoves(const std::string& FEN)
{
	return engine.getLegalMoves(FEN);
}
