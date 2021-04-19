#include "MenuHandler.h"

#include "Window.h"
#include "Resources.h"
#include "Drawable.h"
#include "ThirdPartyWrappersFactory.h"


std::optional<MenuResult> MenuHandler::run(Window& window)
{
	init(window);

	const auto difficulty = getDifficultyLevelFromUser(window);
	if (!difficulty)
	{
		// Quit.
		return {};
	}

	const auto side = getSideFromUser(window);
	if (!side)
	{
		// Quit.
		return {};
	}

	return MenuResult{*difficulty, *side};
}

void MenuHandler::init(const Window& window)
{
	background = ThirdPartyWrappersFactory::createDrawable(Resources::getMenuBackgroundImg());
	background->setScale(Vec2<float>(1.f / 1.5f, 1.f / 1.5f));

	auto createButton = [](const ResourceData& data, const Vec2<float>& scale, const Vec2<float>& normPos,
		const Window& window)
	{
		auto button = ThirdPartyWrappersFactory::createClickable(data);
		button->getDrawable().setScale(scale);
		button->getDrawable().setNormalizedPosition(normPos, window);
		return button;
	};

	static constexpr float buttonRelOffsX = 0.07;
	static constexpr float topButtonRelOffsY = 0.06;
	static constexpr float buttonToButtonOffsY = 0.2;
	static const Vec2<float> difficultyButtonScale(0.5f, 0.5f);
	static const Vec2<float> sideButtonScale(1.f / 1.5f, 1.f / 1.5f);

	buttonHard = createButton(Resources::getMenuButtonHardImg(), difficultyButtonScale,
		Vec2<float>(buttonRelOffsX, topButtonRelOffsY), window);

	buttonMedium = createButton(Resources::getMenuButtonMediumImg(), difficultyButtonScale,
		Vec2<float>(buttonRelOffsX, topButtonRelOffsY + 1 * buttonToButtonOffsY), window);

	buttonEasy = createButton(Resources::getMenuButtonEasyImg(), difficultyButtonScale,
		Vec2<float>(buttonRelOffsX, topButtonRelOffsY + 2 * buttonToButtonOffsY), window);

	buttonSilly = createButton(Resources::getMenuButtonSillyImg(), difficultyButtonScale,
		Vec2<float>(buttonRelOffsX, topButtonRelOffsY + 3 * buttonToButtonOffsY), window);

	buttonSideWhite = createButton(Resources::getMenuButtonSideWhiteImg(), sideButtonScale,
		Vec2<float>(0.f, 0.f), window);

	buttonSideBlack = createButton(Resources::getMenuButtonSideBlackImg(), sideButtonScale,
		Vec2<float>(0.5f, 0.f), window);
}

void MenuHandler::drawDefaultMenu(Window& window)
{
	window.clear();
	window.draw(*background);
	window.draw(buttonHard->getDrawable());
	window.draw(buttonMedium->getDrawable());
	window.draw(buttonEasy->getDrawable());
	window.draw(buttonSilly->getDrawable());
	window.display();
}

void MenuHandler::drawSideSelectionMenu(Window& window)
{
	window.clear();
	window.draw(buttonSideWhite->getDrawable());
	window.draw(buttonSideBlack->getDrawable());
	window.display();
}

std::optional<statesAndEvents::DifficultyLevel>
	MenuHandler::getDifficultyLevelFromUser(Window& window)
{
	using namespace statesAndEvents;

	while (true)
	{
		drawDefaultMenu(window);

		Event event = window.pollEvent();
		if (event.type == EventType::Close)
		{
			// Quit.
			return {};
		}
		else if (event.type != EventType::MouseDown)
		{
			continue;
		}

		// Event.type == MouseDown.

		if (buttonHard->wasClicked(Vec2<int32_t>(event.mouseX, event.mouseY), window))
		{
			return DifficultyLevel::Hard;
		}

		if (buttonMedium->wasClicked(Vec2<int32_t>(event.mouseX, event.mouseY), window))
		{
			return DifficultyLevel::Medium;
		}

		if (buttonEasy->wasClicked(Vec2<int32_t>(event.mouseX, event.mouseY), window))
		{
			return DifficultyLevel::Easy;
		}

		if (buttonSilly->wasClicked(Vec2<int32_t>(event.mouseX, event.mouseY), window))
		{
			return DifficultyLevel::Silly;
		}
	}
}

std::optional<statesAndEvents::PlayingSide> MenuHandler::getSideFromUser(Window& window)
{
	using namespace statesAndEvents;

	while (true)
	{
		drawSideSelectionMenu(window);

		Event event = window.pollEvent();
		if (event.type == EventType::Close)
		{
			// Quit.
			return {};
		}
		else if (event.type != EventType::MouseDown)
		{
			continue;
		}

		// Event.type == MouseDown.
		if (buttonSideWhite->wasClicked(Vec2<int32_t>(event.mouseX, event.mouseY), window))
		{
			return PlayingSide::White;
		}

		if (buttonSideBlack->wasClicked(Vec2<int32_t>(event.mouseX, event.mouseY), window))
		{
			return PlayingSide::Black;
		}
	}
}
