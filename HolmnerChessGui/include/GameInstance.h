#pragma once

#include "Window.h"

#include <memory>

class GameInstance
{
public:
	void run();

private:
	void init();

	std::unique_ptr<Window> window;
};
