#pragma once

#include "GameStateHandler.h"

#include <memory>

class Drawable;

class MenuHandler : public GameStateHandler
{
public:
	MenuHandler();

	gameState::GameState run(Window& window) override;

private:
	void draw(Window& window);

	std::unique_ptr<Drawable> background;
};
