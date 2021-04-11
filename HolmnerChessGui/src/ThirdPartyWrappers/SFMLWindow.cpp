#include "ThirdPartyWrappers/SFMLWindow.h"

#include "GuiUtilities.h"

#include "ThirdPartyWrappers/SFMLDrawable.h"

#include <cassert>

void SFMLWindow::open()
{
	//static constexpr uint32_t widthSafetyMargin = 200;
	//static constexpr uint32_t heightSafetyMargin = 200;
	//const auto mode = sf::VideoMode::getDesktopMode();
	//uint32_t finalWidth = mode.width - widthSafetyMargin;
	//uint32_t finalHeight = mode.height - heightSafetyMargin;

	//// Maintain default aspect ratio.
	//if ((float)finalWidth / (float)finalHeight > Window::defaultWidthHeightRatio)
	//{
	//	finalWidth = (uint32_t)((float)finalHeight * Window::defaultWidthHeightRatio);
	//}
	//else
	//{
	//	finalHeight = (uint32_t)((float)finalWidth / Window::defaultWidthHeightRatio);
	//}

	//// Create the window with the default size, then resize the window to the final size.
	//// This is done to ensure that everything is scaled correctly.
	//window.create(sf::VideoMode(Window::defaultWidth, Window::defaultHeight),
	//	"Holmner Chess Engine", sf::Style::Close);

	//// Update the size of the window to the final size and call onResize which performs letterbox
	//// scaling.
	//window.setSize(sf::Vector2u(finalWidth, finalHeight));
	//onResize(finalWidth, finalHeight);
	//window.setPosition(sf::Vector2i(0, 0));

	window.create(sf::VideoMode(720, 450),
			"Holmner Chess Engine");
	window.setSize(sf::Vector2u(1440, 900));
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, 1440, 900)));
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
		case sf::Event::Resized:
			onResize(sfmlEvent.size.width, sfmlEvent.size.height);
			event.type = statesAndEvents::EventType::None;
			return event;
		default:
			event.type = statesAndEvents::EventType::None;
			return event;
	}
}

namespace
{
	// The follosing function is copied from:
	// https://github.com/SFML/SFML/wiki/Source:-Letterbox-effect-using-a-view
	sf::View getLetterboxView(sf::View view, int32_t windowWidth, int32_t windowHeight) {

		// Compares the aspect ratio of the window to the aspect ratio of the view,
		// and sets the view's viewport accordingly in order to archieve a letterbox effect.
		// A new view (with a new viewport set) is returned.
		const float windowRatio = windowWidth / (float)windowHeight;
		const float viewRatio = view.getSize().x / (float)view.getSize().y;
		float sizeX = 1;
		float sizeY = 1;
		float posX = 0;
		float posY = 0;

		const bool horizontalSpacing = windowRatio >= viewRatio;

		// If horizontalSpacing is true, the black bars will appear on the left and right side.
		// Otherwise, the black bars will appear on the top and bottom.
		if (horizontalSpacing)
		{
			sizeX = viewRatio / windowRatio;
			posX = (1 - sizeX) / 2.f;
		}
		else
		{
			sizeY = windowRatio / viewRatio;
			posY = (1 - sizeY) / 2.f;
		}

		view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
		return view;
	}
}

void SFMLWindow::onResize(uint32_t newWidth, uint32_t newHeight)
{
	window.setView(getLetterboxView(window.getView(), newWidth, newHeight));
}
