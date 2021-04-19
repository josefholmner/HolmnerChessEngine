#pragma once

#include "Window.h"

#include "SFML/Graphics.hpp"

class SFMLWindow : public Window
{
public:
	void open(const Vec2<uint32_t>& size) override;
	void close() override;
	bool isOpen() const override;

	Vec2<uint32_t> getSize() const override;

	void draw(const Drawable& drawable) override;
	void draw(const Text& text) override;

	void clear() override;
	void display() override;

	Vec2<float> pixelToNormalizedPosition(const Vec2<int32_t>& pixel) const override;
	Vec2<float> normalizedPositionToCoordinate(const Vec2<float> normPos) const;

	Vec2<int32_t> getMousePos() const override;

	statesAndEvents::Event pollEvent() override;

	const sf::RenderWindow& getRenderWindow() const
	{
		return window;
	}

private:
	void onResize(uint32_t newWidth, uint32_t newHeight);

	sf::RenderWindow window;
};
