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
	using namespace statesAndEvents;

	init(window);

	while (true)
	{
		draw(window);

		Event event = window.pollEvent();
		if (event.type == EventType::Close)
		{
			// Quit.
			return {};
		}
		else if (event.type == EventType::MouseDown)
		{
			board.registerMouseDown(Vec2<int32_t>(event.mouseX, event.mouseY), window);
		}
		else if (event.type == EventType::MouseRelease)
		{
			board.registerMouseRelease(Vec2<int32_t>(event.mouseX, event.mouseY), window);
		}
	}

	return {};
}

void PlayHandler::init(const Window& window)
{
	static const Vec2<float> boardPos(0.25f, 0.05f);
	static const Vec2<float> boardScale(0.5f, 0.5f);
	board.init(boardPos, boardScale, window);
}

void PlayHandler::draw(Window& window)
{
	window.clear();
	board.draw(window, window.getMousePos());
	window.display();
}
