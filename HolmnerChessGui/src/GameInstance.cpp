#include "GameInstance.h"

#include "GuiUtilities.h"
#include "ThirdPartyWrappersFactory.h"
#include "StatesAndEvents.h"
#include "MenuHandler.h"
#include "PlayHandler.h"

#include <cassert>


void GameInstance::run()
{
	assert(window != nullptr && window->isOpen());

	MenuHandler menuHandler;
	const auto menuResult = menuHandler.run(*window);
	if (!menuResult)
	{
		// Quit.
		window->close();
		return;
	}

	PlayHandler playHandler;
	const auto playResult = playHandler.run(*window, menuResult->difficulty, menuResult->side);
	if (!playResult)
	{
		// Quit.
		window->close();
		return;
	}

	if (playResult->nextState == statesAndEvents::GameState::Menu)
	{
		run();
	}
}

void GameInstance::init()
{
	window = ThirdPartyWrappersFactory::createWindow();
	window->open();
}
