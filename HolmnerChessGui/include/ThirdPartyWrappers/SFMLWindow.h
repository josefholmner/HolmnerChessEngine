#pragma once

#include "Window.h"

#include "SFML/Graphics.hpp"

class SFMLWindow : public Window
{
public:
	void open(const Vec2<uint32_t>& size) override;
	void close() override;
	bool isOpen() override;

	void draw(const Drawable& drawable) override;
	void clear() override;
	void display() override;

private:
	sf::RenderWindow window;
};
