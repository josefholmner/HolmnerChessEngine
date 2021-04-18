#pragma once

#include "Engine/EngineAPI.h"

#include <string>
#include <future>
#include <optional>

class EngineWrapper
{
public:
	hceEngine::LegalMovesCollection getLegalMoves(const std::string& FEN);

	// Async search.
	bool startSearchDepth(const std::string& FEN, uint8_t depth);
	bool startSearchTimeout(const std::string& FEN, int32_t mills);
	bool startSearchMiniMax(const std::string& FEN, uint8_t depth);
	bool startSearchWorstMoveMiniMax(const std::string& FEN, uint8_t depth);

	bool isProcessingRequest() const;
	std::optional<hceEngine::SearchResult> getSearchResult();	

private:
	hceEngine::SearchResult getBestMoveDepth(const std::string& FEN, uint8_t depth);
	hceEngine::SearchResult getBestMoveTimeout(const std::string& FEN, int32_t mills);
	hceEngine::SearchResult getBestMoveMiniMax(const std::string& FEN, uint8_t depth);
	hceEngine::SearchResult getWorstMoveMiniMax(const std::string& FEN, uint8_t depth);

	hceEngine::EngineAPI engine;
	std::future<hceEngine::SearchResult> future;
};
