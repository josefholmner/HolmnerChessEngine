#pragma once

#include "Window.h"

#include "SFML/Graphics.hpp"

class SFMLWindow : public Window
{
public:
	void open() override;
	void close() override;
	bool isOpen() const override;

	Vec2<uint32_t> getSize() const override;

	void draw(const Drawable& drawable) override;
	void clear() override;
	void display() override;

	statesAndEvents::Event pollEvent() override;

	const sf::RenderWindow& getRenderWindow() const
	{
		return window;
	}

private:
	void onResize(uint32_t newWidth, uint32_t newHeight);

	sf::RenderWindow window;
};
