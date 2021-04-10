#include "ThirdPartyWrappers/SFMLClickable.h"

#include "GuiUtilities.h"

#include "ThirdPartyWrappers/SFMLWindow.h"

SFMLClickable::SFMLClickable(const ImageData& image) : drawable(image)
{
}

bool SFMLClickable::wasClicked(const Vec2<int32_t>& mousePos, const Window& window)
{
	const SFMLWindow* sfmlWindow = dynamic_cast<const SFMLWindow*>(&window);
	if (sfmlWindow == nullptr)
	{
		GuiUtilities::logE("Non SFML Window passed to SFMLClickable::wasCliced(). Dynamic cast failed.");
		return false;
	}

	const sf::RenderWindow& renderWindow = sfmlWindow->getRenderWindow();
	const auto globalMousePos = renderWindow.mapPixelToCoords(sf::Vector2i(mousePos.x(), mousePos.y()));
	return drawable.getSprite().getGlobalBounds().contains(globalMousePos.x, globalMousePos.y);
}

Drawable& SFMLClickable::getDrawable()
{
	return drawable;
}
