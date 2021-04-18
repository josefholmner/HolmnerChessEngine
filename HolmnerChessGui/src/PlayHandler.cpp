#include "PlayHandler.h"

#include "Resources.h"
#include "Window.h"
#include "GuiUtilities.h"
#include "Common/StopWatch.h"

PlayHandler::PlayHandler()
	: board(Resources::getBoardImg())
{
}

std::optional<PlayResult> PlayHandler::run(Window& window, statesAndEvents::DifficultyLevel difficulty,
	statesAndEvents::PlayingSide side)
{
	init(window, side);

	bool isPlaying = true;
	while (isPlaying)
	{
		const auto legalMoves = engine.getLegalMoves(board.getFEN());
		if (legalMoves.moves.size() == 0)
		{
			return showEndScreen(legalMoves.state, window);
		}

		if (isUsersTurn(legalMoves.state))
		{
			isPlaying = userMakeMove(legalMoves, window);
		}
		else
		{
			isPlaying = engineMakeMove(window, difficulty);
		}
	}

	return {};
}

void PlayHandler::init(const Window& window, statesAndEvents::PlayingSide side)
{
	static const Vec2<float> boardPos(0.25f, 0.05f);
	static const Vec2<float> boardScale(0.5f, 0.5f);
	board.init(boardPos, boardScale, side, window);
}

void PlayHandler::draw(Window& window)
{
	window.clear();
	board.draw(window, window.getMousePos());
	window.display();
}

namespace
{
	std::optional<hceEngine::ChessMove> findMoveFromUserMove(
		const UserMove& userMove, const hceEngine::LegalMovesCollection& legalMoves)
	{
		for (const auto& move : legalMoves.moves)
		{
			if (userMove.fromSquare == move.fromSquare && userMove.toSquare == move.toSquare)
			{
				return move;
			}
		}

		return {};
	}
}

bool PlayHandler::userMakeMove(const hceEngine::LegalMovesCollection& legalMoves, Window& window)
{
	using namespace statesAndEvents;

	while (true)
	{
		draw(window);

		Event event = window.pollEvent();
		if (event.type == EventType::Close)
		{
			// Quit.
			return false;
		}
		else if (event.type == EventType::MouseDown)
		{
			board.registerMouseDown(Vec2<int32_t>(event.mouseX, event.mouseY), window);
		}
		else if (event.type == EventType::MouseRelease)
		{
			if (const auto userMove =
				board.registerMouseRelease(Vec2<int32_t>(event.mouseX, event.mouseY), window))
			{
				if (const auto move = findMoveFromUserMove(*userMove, legalMoves))
				{
					board.makeMove(*move, window);
					consolePrintUserMoveInfo(*move);
					return true;
				}
			}
		}
	}
}

bool PlayHandler::engineMakeMove(Window& window, statesAndEvents::DifficultyLevel difficulty)
{
	using namespace statesAndEvents;

	// Start async engine search.
	if (!startEngineSearch(difficulty))
	{
		GuiUtilities::logE("Unexpected error, unable to start engine search.");
		return false;
	}

	static constexpr int32_t minThinkingTime = 2000;
	hceCommon::Stopwatch stopWatch;
	stopWatch.start();

	while (true)
	{
		draw(window);

		Event event = window.pollEvent();
		if (event.type == EventType::Close)
		{
			// Quit.
			return false;
		}

		// Enforce minumum "thinking time" for the engine for more enjoyable play.
		if (stopWatch.getMilliseconds() >= minThinkingTime && !engine.isProcessingRequest())
		{
			if (const auto searchResult = engine.getSearchResult())
			{
				// Engine search result is ready.
				board.makeMove(searchResult->move, window);
				consolePrintEngineMoveInfo(*searchResult);
				return true;
			}
		}
	}
}

bool PlayHandler::startEngineSearch(statesAndEvents::DifficultyLevel difficulty)
{
	switch (difficulty)
	{
		case statesAndEvents::DifficultyLevel::Hard:
			return engine.startSearchTimeout(board.getFEN(), 40000);
		case statesAndEvents::DifficultyLevel::Medium:
			return engine.startSearchDepth(board.getFEN(), 4);
			break;
		case statesAndEvents::DifficultyLevel::Easy:
			return engine.startSearchMiniMax(board.getFEN(), 2);
			break;
		case statesAndEvents::DifficultyLevel::Silly:
			return engine.startSearchWorstMoveMiniMax(board.getFEN(), 3);
			return false;
			break;
		default:
			return false;
	}
}

void PlayHandler::consolePrintUserMoveInfo(const hceEngine::ChessMove& move) const
{
	GuiUtilities::log("User: " + move.fromSquare + move.toSquare);
}

void PlayHandler::consolePrintEngineMoveInfo(const hceEngine::SearchResult& searchResult) const
{
		const std::string moveStr = searchResult.move.fromSquare + searchResult.move.toSquare;
		const std::string depth = std::to_string(searchResult.engineInfo.depthsCompletelyCovered);
		const std::string maxDepth = std::to_string(searchResult.engineInfo.maxDepthVisited);
		const std::string nodes = std::to_string(searchResult.engineInfo.nodesVisited);
		GuiUtilities::log("Engine: " + moveStr + ", depth: " + depth + ", max depth: " + maxDepth +
		", nodes visited: " + nodes);
}

std::optional<PlayResult> PlayHandler::showEndScreen(hceEngine::PlayState state, Window& window)
{
	using namespace statesAndEvents;
	while (true)
	{
		draw(window);

		Event event = window.pollEvent();
		if (event.type == EventType::Close)
		{
			// Quit.
			return {};
		}
		
		// TODO: implement end-screen.
	}
}

bool PlayHandler::isUsersTurn(hceEngine::PlayState state)
{
	if (state == hceEngine::PlayState::WhiteToMove &&
		board.getUserSide() == statesAndEvents::PlayingSide::White)
	{
		return true;
	}

	if (state == hceEngine::PlayState::BlackToMove &&
		board.getUserSide() == statesAndEvents::PlayingSide::Black)
	{
		return true;
	}

	return false;
}
