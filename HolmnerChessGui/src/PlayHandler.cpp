#include "PlayHandler.h"

#include "Resources.h"
#include "Window.h"

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
			return showEndScreen(window);
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
					return true;
				}
			}
		}
	}
}

bool PlayHandler::engineMakeMove(Window& window)
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

		// TODO: implement engine make move.
	}
}

std::optional<PlayResult> PlayHandler::showEndScreen(Window& window)
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
