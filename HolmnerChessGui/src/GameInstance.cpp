#include "GameInstance.h"

#include "ThirdPartyWrappersFactory.h"

#include <assert.h>

void GameInstance::startNew()
{
	window = ThirdPartyWrappersFactory::createWindow();
	window->open();

	run();
}

void GameInstance::run()
{
	assert(window != nullptr);

	// Main game loop.
	while (window->isOpen())
	{
		// TODO: Add game logic.
	}
}
