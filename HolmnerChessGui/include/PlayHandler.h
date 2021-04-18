#pragma once

#include "StatesAndEvents.h"
#include "Board.h"
#include "EngineWrapper.h"

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
	void init(const Window& window, statesAndEvents::PlayingSide side);

	void draw(Window& window);

	bool userMakeMove(const hceEngine::LegalMovesCollection& legalMoves, Window& window);
	bool engineMakeMove(Window& window, statesAndEvents::DifficultyLevel difficulty);

	bool startEngineSearch(statesAndEvents::DifficultyLevel difficulty);

	std::optional<PlayResult> showEndScreen(hceEngine::PlayState state, Window& window);

	bool isUsersTurn(hceEngine::PlayState state);

	Board board;
	EngineWrapper engine;
};
