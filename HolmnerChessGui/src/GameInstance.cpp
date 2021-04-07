#include "GameInstance.h"

#include "GuiUtilities.h"
#include "ThirdPartyWrappersFactory.h"
#include "MenuHandler.h"

#include <cassert>


void GameInstance::run()
{
	using namespace gameState;

	init();
	assert(window != nullptr && window->isOpen());

	MenuHandler menu;

	// Main game loop.
	GameState state = GameState::Menu;
	while (window->isOpen())
	{
		switch (state)
		{
			case GameState::Menu:
				state = menu.run(*window);
				break;
			case GameState::PlaySilly:
				break;
			case GameState::PlayEasy:
				break;
			case GameState::PlayMedium:
				break;
			case GameState::PlayHard:
				break;
			case GameState::Quit:
				window->close();
				return;
			default:
				GuiUtilities::logE("Unknown game state in GameInstance::run().");
				state = GameState::Quit;
				break;
		}
	}
}

void GameInstance::init()
{
	window = ThirdPartyWrappersFactory::createWindow();
	window->open();
}
