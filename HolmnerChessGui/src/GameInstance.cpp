#include "GameInstance.h"

#include "GuiUtilities.h"
#include "ThirdPartyWrappersFactory.h"
#include "StatesAndEvents.h"
#include "MenuHandler.h"

#include <cassert>


void GameInstance::run()
{
	init();
	assert(window != nullptr && window->isOpen());

	MenuHandler menu;
	const auto menuResult = menu.run(*window);
	if (!menuResult)
	{
		// Quit.
		window->close();
		return;
	}
}

void GameInstance::init()
{
	window = ThirdPartyWrappersFactory::createWindow();
	window->open();
}
