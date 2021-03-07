#pragma once

#include "Window.h"

#include "SFML/Graphics.hpp"

class SFMLWindow : public Window
{
public:
	void open(uint32_t width, uint32_t height) override;

	bool isOpen() override;

private:
	sf::RenderWindow renderWindow;
};
