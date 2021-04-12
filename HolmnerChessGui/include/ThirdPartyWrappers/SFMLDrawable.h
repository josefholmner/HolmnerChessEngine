#pragma once

#include "Drawable.h"

#include "SFML/Graphics.hpp"

#include <cstdint>

struct ImageData;

class SFMLDrawable : public Drawable
{
public:
	SFMLDrawable(const ImageData& image);

	void setNormalizedPosition(const Vec2<float>& normPos, const Window& window) override;
	Vec2<float> getNormalizedPosition(const Window& window) override;

	Vec2<float> getNormalizedSize(const Window& window) const override;

	void setScale(const Vec2<float>& scale) override;
	Vec2<float> getScale() const override;

	const sf::Sprite& getSprite() const { return sprite; }

private:
	sf::Texture texture;
	sf::Sprite sprite;
};
