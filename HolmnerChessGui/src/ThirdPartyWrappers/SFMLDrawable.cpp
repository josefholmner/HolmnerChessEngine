#include "ThirdPartyWrappers/SFMLDrawable.h"

#include "GuiUtilities.h"
#include "ImageData.h"
#include "Window.h"

#include "ThirdPartyWrappers/SFMLWindow.h"

SFMLDrawable::SFMLDrawable(const ImageData& image)
{
	if (!texture.loadFromMemory(image.data, image.length))
	{
		GuiUtilities::logE("SFMLDrawable texture.loadFromMemory() failed.");
		return;
	}

	texture.setSmooth(true);
	sprite.setTexture(texture);
}

void SFMLDrawable::setRelativePosition(const Vec2<float>& relPos, const Window& window)
{
	const SFMLWindow* sfmlWindow = dynamic_cast<const SFMLWindow*>(&window);
	if (sfmlWindow == nullptr)
	{
		GuiUtilities::logE("Non SFML Window passed to SFMLDrawable::setRelativePosition().");
		return;
	}

	const sf::RenderWindow& renderWindow = sfmlWindow->getRenderWindow();
	const auto windowSize = window.getSize();

	const Vec2<float> pixel(relPos.x() * (float)windowSize.x(), relPos.y() * (float)windowSize.y());
	const auto coordinate = 
		renderWindow.mapPixelToCoords(sf::Vector2i((int32_t)pixel.x(), (int32_t)pixel.y()));

	sprite.setPosition((float)coordinate.x, (float)coordinate.y);
}

void SFMLDrawable::setScale(const Vec2<float>& scale)
{
	sprite.setScale(scale.x(), scale.y());
}

Vec2<float> SFMLDrawable::getScale() const
{
	const auto scale = sprite.getScale();
	return Vec2<float>(scale.x, scale.y);
}
