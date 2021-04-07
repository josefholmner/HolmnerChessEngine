#include "MenuHandler.h"

#include "Window.h"
#include "Resources.h"
#include "Drawable.h"
#include "ThirdPartyWrappersFactory.h"

MenuHandler::MenuHandler()
{
	background = ThirdPartyWrappersFactory::createDrawable(Resources::getMenuBackgroundImg());
}

gameState::GameState MenuHandler::run(Window& window)
{
	using namespace gameState;

	GameState nextState = GameState::Menu;
	while (window.isOpen())
	{
		draw(window);
	}

	return nextState;
}

void MenuHandler::draw(Window& window)
{
	window.clear();
	window.draw(*background);
	window.display();
}
