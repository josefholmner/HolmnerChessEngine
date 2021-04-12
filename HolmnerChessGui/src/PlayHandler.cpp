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
	}

	return {};
}

void PlayHandler::init(const Window& window)
{
	static constexpr float boardPosX = 0.25f;
	static constexpr float boardPosY = 0.05f;
	static constexpr float boardScale = 0.5f;
	board.setNormalizedPosition(Vec2<float>(boardPosX, boardPosY), window);
	board.setScale(Vec2<float>(boardScale, boardScale));
}

void PlayHandler::draw(Window& window)
{
	window.clear();
	board.draw(window);
	window.display();
}
