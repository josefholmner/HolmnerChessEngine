#include "ThirdPartyWrappers/SFMLText.h"

#include "GuiUtilities.h"
#include "Window.h"

#include "ThirdPartyWrappers/SFMLWindow.h"

SFMLText::SFMLText(const std::string& str, const ResourceData& inFont, uint32_t sizeInPixels)
{
	if (!font.loadFromMemory(inFont.data, inFont.length))
	{
		GuiUtilities::logE("SFMLText font.loadFromMemory() failed.");
		return;
	}

	setText(str);
	text.setFont(font);
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(sizeInPixels);
}

void SFMLText::setText(const std::string& str)
{
	text.setString(sf::String(str));
}

void SFMLText::setNormalizedPosition(const Vec2<float>& normPos, const Window& window)
{
	const SFMLWindow* sfmlWindow = dynamic_cast<const SFMLWindow*>(&window);
	if (sfmlWindow == nullptr)
	{
		GuiUtilities::logE("Non SFML Window passed to SFMLText::setNormalizedPosition().");
		return;
	}

	const auto coordinate = sfmlWindow->normalizedPositionToCoordinate(normPos);
	text.setPosition(coordinate.x(), coordinate.y());
}
