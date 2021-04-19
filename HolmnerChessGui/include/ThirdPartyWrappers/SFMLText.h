#pragma once

#include "Text.h"
#include "ResourceData.h"

#include "SFML/Graphics.hpp"


class SFMLText : public Text
{
public:
	SFMLText(const std::string& str, const ResourceData& inFont, uint32_t sizeInPixels);

	void setText(const std::string& str) override;
	const sf::Text& getText() const { return text; }

	void setNormalizedPosition(const Vec2<float>& normPos, const Window& window) override;
private:
	sf::Text text;
	sf::Font font;
};
