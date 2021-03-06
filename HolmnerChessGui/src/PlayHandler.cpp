#include "PlayHandler.h"

#include "Resources.h"
#include "Window.h"
#include "Clickable.h"
#include "Drawable.h"
#include "GuiUtilities.h"
#include "ThirdPartyWrappersFactory.h"
#include "ResourceData.h"
#include "Common/StopWatch.h"

#include <cassert>

PlayHandler::PlayHandler()
	: board(Resources::getBoardImg()),
	difficulty{statesAndEvents::DifficultyLevel::Silly}
{
}

std::optional<PlayResult> PlayHandler::run(Window& window, statesAndEvents::DifficultyLevel inDifficulty,
	statesAndEvents::PlayingSide side)
{
	difficulty = inDifficulty;
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
			isPlaying = engineMakeMove(window);
		}
	}

	return {};
}

void PlayHandler::init(const Window& window, statesAndEvents::PlayingSide side)
{
	static const Vec2<float> boardPos(0.255f, 0.105f);
	static const Vec2<float> boardScale(0.475f, 0.475f);
	static const Vec2<float> boardEdgeScale(1.f / 1.5f, 1.f / 1.5f);
	board.init(boardPos, boardScale, side, window);
	
	if (side == statesAndEvents::PlayingSide::White)
	{
		boardEdge = ThirdPartyWrappersFactory::createDrawable(Resources::getBoardEdgeWhiteImg());
	}
	else
	{
		boardEdge = ThirdPartyWrappersFactory::createDrawable(Resources::getBoardEdgeBlackImg());
	}
	
	boardEdge->setScale(boardEdgeScale);

	whiteWinsImg = ThirdPartyWrappersFactory::createClickable(Resources::getWhiteWinsImg());
	blackWinsImg = ThirdPartyWrappersFactory::createClickable(Resources::getBlackWinsImg());
	drawImg = ThirdPartyWrappersFactory::createClickable(Resources::getDrawImg());
	static const Vec2<float> endGameButtonPos(0.37f, 0.44f);
	for (auto& b : { whiteWinsImg.get(), blackWinsImg.get(), drawImg.get() })
	{
		b->getDrawable().setNormalizedPosition(endGameButtonPos, window);
	}

	setPlayInfoText();
}

void PlayHandler::draw(Window& window, statesAndEvents::PlayingSide turn)
{
	assert(boardEdge != nullptr);

	window.clear();
	window.draw(*boardEdge);
	board.draw(window, window.getMousePos(), turn);
	window.display();
}

void PlayHandler::drawEndScreen(Window& window, const Drawable& button)
{
	assert(boardEdge != nullptr);

	window.clear();
	window.draw(*boardEdge);
	board.draw(window, window.getMousePos(), statesAndEvents::PlayingSide::None);
	window.draw(button);
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
		draw(window, board.getUserSide());

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

bool PlayHandler::engineMakeMove(Window& window)
{
	using namespace statesAndEvents;

	// Start async engine search.
	if (!startEngineSearch())
	{
		GuiUtilities::logE("Unexpected error, unable to start engine search.");
		return false;
	}

	const PlayingSide engineSide = board.getUserSide() == PlayingSide::White ?
		PlayingSide::Black : PlayingSide::White;

	static constexpr int32_t minThinkingTime = 2000;
	hceCommon::Stopwatch stopWatch;
	stopWatch.start();

	while (true)
	{
		draw(window, engineSide);

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

bool PlayHandler::startEngineSearch()
{
	switch (difficulty)
	{
		case statesAndEvents::DifficultyLevel::Hard:
			// Avoid unnecessary (and boring) long searches at first moves.
			if (board.getMoveCount() <= 1)
			{
				return engine.startSearchDepthTimeout(board.getFEN(), 8, 20000);
			}
			else if (board.getMoveCount() <= 3)
			{
				return engine.startSearchDepthTimeout(board.getFEN(), 9, 20000);
			}
			else if (board.getMoveCount() <= 5)
			{
				return engine.startSearchDepthTimeout(board.getFEN(), 9, 30000);
			}
			else
			{
				return engine.startSearchTimeout(board.getFEN(), 40000);
			}
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
		const std::string eval = std::to_string(searchResult.move.positionEvaluation);
		GuiUtilities::log("Engine: " + moveStr + ", depth covered: " + depth + ", max depth: " +
			maxDepth + ", nodes visited: " + nodes + ", evaluation: " + eval);
}

void PlayHandler::setPlayInfoText()
{
	std::string info = "Difficulty: ";
	switch (difficulty)
	{
		case statesAndEvents::DifficultyLevel::Hard:
			info += "Hard";
			break;
		case statesAndEvents::DifficultyLevel::Medium:
			info += "Medium";
			break;
		case statesAndEvents::DifficultyLevel::Easy:
			info += "Easy";
			break;
		case statesAndEvents::DifficultyLevel::Silly:
			info += "Silly";
			break;
	}

	board.setPlayInfoText(info);
}

std::optional<PlayResult> PlayHandler::showEndScreen(hceEngine::PlayState state, Window& window)
{
	using namespace statesAndEvents;
	assert(whiteWinsImg != nullptr);
	assert(blackWinsImg != nullptr);
	assert(drawImg != nullptr);

	auto getButton = [&](hceEngine::PlayState state) -> std::unique_ptr<Clickable>&
	{
		if (state == hceEngine::PlayState::WhiteWins)
		{
			GuiUtilities::log("White wins\n");
			return whiteWinsImg;
		}
		else if (state == hceEngine::PlayState::BlackWins)
		{
			GuiUtilities::log("Black wins\n");
			return blackWinsImg;
		}
		else
		{
			GuiUtilities::log("Draw\n");
			return drawImg;
		}
	};

	auto& button = getButton(state);

	while (true)
	{
		drawEndScreen(window, button->getDrawable());

		Event event = window.pollEvent();
		if (event.type == EventType::Close)
		{
			// Quit.
			return {};
		}
		else if (event.type != EventType::MouseDown)
		{
			continue;
		}

		// Event.type == MouseDown.

		if (button->wasClicked(Vec2<int32_t>(event.mouseX, event.mouseY), window))
		{
			PlayResult res{ statesAndEvents::GameState::Menu };
			return res;
		}
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
