#include "ThirdPartyWrappers/SFMLWindow.h"

#include "GuiUtilities.h"

#include "ThirdPartyWrappers/SFMLDrawable.h"

#include <cassert>

void SFMLWindow::open(const Vec2<uint32_t>& size)
{
	window.create(sf::VideoMode(size.x(), size.y()), "Holmner Chess Engine");
}

void SFMLWindow::close()
{
	window.close();
}

bool SFMLWindow::isOpen()
{
	return window.isOpen();
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
