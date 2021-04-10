#pragma once

#include "StatesAndEvents.h"
#include "Clickable.h"

#include <memory>
#include <optional>

class Drawable;

struct MenuResult
{
	statesAndEvents::DifficultyLevel difficulty;
	statesAndEvents::PlayingSide side;
};

class MenuHandler
{
public:
	std::optional<MenuResult> run(Window& window);

private:
	void init(const Window& window);

	void drawDefaultMenu(Window& window);
	void drawSideSelectionMenu(Window& window);
	std::optional<statesAndEvents::DifficultyLevel> getDifficultyLevelFromUser(Window& window);
	std::optional<statesAndEvents::PlayingSide> getSideFromUser(Window& window);

	std::unique_ptr<Drawable> background;
	std::unique_ptr<Clickable> buttonHard;
	std::unique_ptr<Clickable> buttonMedium;
	std::unique_ptr<Clickable> buttonEasy;
	std::unique_ptr<Clickable> buttonSilly;
	std::unique_ptr<Clickable> buttonSideWhite;
	std::unique_ptr<Clickable> buttonSideBlack;
};
