#pragma once

#include "StatesAndEvents.h"
#include "Board.h"

#include <optional>

class Window;

struct PlayResult
{
	statesAndEvents::GameState nextState;
};

class PlayHandler
{
public:
	PlayHandler();

	std::optional<PlayResult> run(Window& window, statesAndEvents::DifficultyLevel difficulty,
		statesAndEvents::PlayingSide side);

private:
	void init(const Window& window);
	void draw(Window& window);

	Board board;
};
