#include "ThirdPartyWrappers/SFMLWindow.h"

#include "GuiUtilities.h"
#include "Text.h"

#include "ThirdPartyWrappers/SFMLDrawable.h"
#include "ThirdPartyWrappers/SFMLText.h"

#include <cassert>

void SFMLWindow::open(const Vec2<uint32_t>& size)
{
	// Create the window at a lower resolution, then upscale to the final size.
	// This is done due to some scaling issues on low resolution screens.
	const uint32_t safeWidth = 800;
	const uint32_t safeHeight = 600;
	window.create(sf::VideoMode(safeWidth, safeHeight), "Holmner Chess Engine");
	window.setPosition(sf::Vector2i(0, 0));

	window.setSize(sf::Vector2u(size.x(), size.y()));
	window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.x(), size.y())));
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

void SFMLWindow::draw(const Text& text)
{
	const SFMLText* sfmlText = dynamic_cast<const SFMLText*>(&text);
	if (sfmlText == nullptr)
	{
		GuiUtilities::logE("Non SFML Text passed to SFMLWindow::draw(). Dynamic cast failed.");
		return;
	}

	window.draw(sfmlText->getText());
}

void SFMLWindow::clear()
{
	window.clear();
}

void SFMLWindow::display()
{
	window.display();
}

Vec2<float> SFMLWindow::pixelToNormalizedPosition(const Vec2<int32_t>& pixel) const
{
	const sf::Vector2u windowSize = window.getSize();
	const float viewLeft = window.getView().getViewport().left * windowSize.x;
	const float viewTop = window.getView().getViewport().top * windowSize.y;
	const float viewWidth = window.getView().getViewport().width * windowSize.x;
	const float viewHeight = window.getView().getViewport().height * windowSize.y;

	return Vec2<float>(((float)pixel.x() - viewLeft) / viewWidth,
		((float)pixel.y() - viewTop) / viewHeight);
}

Vec2<float> SFMLWindow::normalizedPositionToCoordinate(const Vec2<float> normPos) const
{
	const auto windowSize = window.getSize();

	// The view size and position must be taken into account since the window is letterbox scaled.
	const float viewLeft = window.getView().getViewport().left * windowSize.x;
	const float viewTop = window.getView().getViewport().top * windowSize.y;
	const float viewWidth = window.getView().getViewport().width * windowSize.x;
	const float viewHeight = window.getView().getViewport().height * windowSize.y;

	const Vec2<float> pixel(normPos.x() * viewWidth + viewLeft, normPos.y() * viewHeight + viewTop);
	const auto coordinate =
		window.mapPixelToCoords(sf::Vector2i((int32_t)pixel.x(), (int32_t)pixel.y()));

	return Vec2<float>(coordinate.x, coordinate.y);
}

Vec2<int32_t> SFMLWindow::getMousePos() const
{
	const auto sfmlPos = sf::Mouse::getPosition(window);
	return Vec2<int32_t>(sfmlPos.x, sfmlPos.y);
}

statesAndEvents::Event SFMLWindow::pollEvent()
{
	statesAndEvents::Event event;

	sf::Event sfmlEvent;
	if (!window.pollEvent(sfmlEvent))
	{
		event.type = statesAndEvents::EventType::None;
		return event;
	}

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
		case sf::Event::MouseButtonReleased:
			event.type = statesAndEvents::EventType::MouseRelease;
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
