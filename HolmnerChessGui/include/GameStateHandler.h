#pragma once

class Window;

namespace gameState
{
	enum class GameState
	{
		Menu,
		PlaySilly,
		PlayEasy,
		PlayMedium,
		PlayHard,
		Quit
	};
}

class GameStateHandler
{
public:
	virtual gameState::GameState run(Window& window) = 0;
};
