#pragma once

#include "Drawable.h"

#include "SFML/Graphics.hpp"

#include <cstdint>

struct ImageData;

class SFMLDrawable : public Drawable
{
public:
	SFMLDrawable(const ImageData& image);

	void setPosition(const Vec2<uint32_t>& pos) override;

	const sf::Sprite& getSprite() const { return sprite; }

private:
	sf::Texture texture;
	sf::Sprite sprite;
};
