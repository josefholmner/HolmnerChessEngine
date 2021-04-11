#include "ThirdPartyWrappers/SFMLWindow.h"

#include "GuiUtilities.h"

#include "ThirdPartyWrappers/SFMLDrawable.h"

#include <cassert>

void SFMLWindow::open(const Vec2<uint32_t>& size)
{
	static uint32_t widthSafetyMargin = 200;
	static uint32_t heightSafetyMargin = 200;
	const auto mode = sf::VideoMode::getDesktopMode();

	window.create(sf::VideoMode(size.x(), size.y()),
		"Holmner Chess Engine");

	window.setSize(sf::Vector2u(mode.width - widthSafetyMargin, mode.height - heightSafetyMargin));
	window.setPosition(sf::Vector2i(0, 0));
}

void SFMLWindow::close()
{
	window.close();
}

bool SFMLWindow::isOpen() const
{
	return window.isOpen();
}

Vec2<uint32_t> SFMLWindow::getSize() const
{
	const auto sfmlSize = window.getSize();
	return Vec2<uint32_t>(sfmlSize.x, sfmlSize.y);
}

void SFMLWindow::draw(const Drawable& drawable)
{
	const SFMLDrawable* sfmlDrawable = dynamic_cast<const SFMLDrawable*>(&drawable);
	if (sfmlDrawable == nullptr)
	{
		GuiUtilities::logE("Non SFML Drawable passed to SFMLWindow::draw(). Dynamic cast failed.");
		return;
	}

	window.draw(sfmlDrawable->getSprite());
}

void SFMLWindow::clear()
{
	window.clear();
}

void SFMLWindow::display()
{
	window.display();
}

statesAndEvents::Event SFMLWindow::pollEvent()
{
	statesAndEvents::Event event;

	sf::Event sfmlEvent;
	window.pollEvent(sfmlEvent);

	switch (sfmlEvent.type)
	{
		case sf::Event::Closed:
			event.type = statesAndEvents::EventType::Close;
			return event;
		case sf::Event::MouseButtonPressed:
			event.type = statesAndEvents::EventType::MouseDown;
			event.mouseX = sfmlEvent.mouseButton.x;
			event.mouseY = sfmlEvent.mouseButton.y;
			return event;
		default:
			event.type = statesAndEvents::EventType::None;
			return event;
	}
}
