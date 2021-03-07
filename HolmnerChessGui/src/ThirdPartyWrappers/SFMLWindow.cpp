#include "ThirdPartyWrappers/SFMLWindow.h"


void SFMLWindow::open(uint32_t width, uint32_t height)
{
	renderWindow.create(sf::VideoMode(width, height), "Holmner Chess Engine");
}

bool SFMLWindow::isOpen()
{
	if (!renderWindow.isOpen())
	{
		return false;
	}

	sf::Event event;
	if (renderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			renderWindow.close();
			return false;
		}
	}

	renderWindow.clear();

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	renderWindow.draw(shape);

	renderWindow.display();

	return true;
}
