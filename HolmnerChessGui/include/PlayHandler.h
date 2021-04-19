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

	std::optional<PlayResult> run(Window& window, statesAndEvents::DifficultyLevel inDifficulty,
		statesAndEvents::PlayingSide side);

private:
	void init(const Window& window, statesAndEvents::PlayingSide side);

	void draw(Window& window);
	void drawPlayStatus(Window& window);

	bool userMakeMove(const hceEngine::LegalMovesCollection& legalMoves, Window& window);
	bool engineMakeMove(Window& window);

	bool startEngineSearch();

	void consolePrintUserMoveInfo(const hceEngine::ChessMove& move) const;
	void consolePrintEngineMoveInfo(const hceEngine::SearchResult& searchResult) const;

	void setPlayInfoText();

	std::optional<PlayResult> showEndScreen(hceEngine::PlayState state, Window& window);

	bool isUsersTurn(hceEngine::PlayState state);

	Board board;
	EngineWrapper engine;
	statesAndEvents::DifficultyLevel difficulty;

};
