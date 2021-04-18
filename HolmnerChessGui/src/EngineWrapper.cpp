#include "EngineWrapper.h"

#include <limits>
#include <chrono>
#include <functional>

hceEngine::LegalMovesCollection EngineWrapper::getLegalMoves(const std::string& FEN)
{
	return engine.getLegalMoves(FEN);
}

bool EngineWrapper::startSearchDepth(const std::string& FEN, uint8_t depth)
{
	if (isProcessingRequest())
	{
		return false;
	}

	auto func = std::bind(
		&EngineWrapper::getBestMoveDepth, this, std::placeholders::_1, std::placeholders::_2);
	future = std::async(std::launch::async, func, FEN, depth);
	return true;
}

bool EngineWrapper::startSearchMiniMax(const std::string& FEN, uint8_t depth)
{
	if (isProcessingRequest())
	{
		return false;
	}

	auto func = std::bind(
		&EngineWrapper::getBestMoveMiniMax, this, std::placeholders::_1, std::placeholders::_2);
	future = std::async(std::launch::async, func, FEN, depth);
	return true;
}

bool EngineWrapper::startSearchTimeout(const std::string& FEN, int32_t mills)
{
	if (isProcessingRequest())
	{
		return false;
	}

	auto func = std::bind(
		&EngineWrapper::getBestMoveTimeout, this, std::placeholders::_1, std::placeholders::_2);
	future = std::async(std::launch::async, func, FEN, mills);
	return true;
}

bool EngineWrapper::startSearchWorstMoveMiniMax(const std::string& FEN, uint8_t depth)
{
	if (isProcessingRequest())
	{
		return false;
	}

	auto func = std::bind(
		&EngineWrapper::getWorstMoveMiniMax, this, std::placeholders::_1, std::placeholders::_2);
	future = std::async(std::launch::async, func, FEN, depth);
	return true;
}

bool EngineWrapper::isProcessingRequest() const
{
	if (!future.valid())
	{
		return false;
	}

	return future.wait_for(std::chrono::seconds(0)) != std::future_status::ready;
}

std::optional<hceEngine::SearchResult> EngineWrapper::getSearchResult()
{
	if (!future.valid() || isProcessingRequest())
	{
		return {};
	}

	return future.get();
}

hceEngine::SearchResult EngineWrapper::getBestMoveTimeout(const std::string& FEN, int32_t mills)
{
	return engine.getBestMove(FEN, std::numeric_limits<uint8_t>::max(), mills);
}

hceEngine::SearchResult EngineWrapper::getBestMoveMiniMax(const std::string& FEN, uint8_t depth)
{
	return engine.getBestMoveMiniMax(FEN, depth);
}

hceEngine::SearchResult EngineWrapper::getWorstMoveMiniMax(const std::string& FEN, uint8_t depth)
{
	return engine.getWorstMoveMiniMax(FEN, depth);
}

hceEngine::SearchResult EngineWrapper::getBestMoveDepth(const std::string& FEN, uint8_t depth)
{
	return engine.getBestMove(FEN, depth);
}
