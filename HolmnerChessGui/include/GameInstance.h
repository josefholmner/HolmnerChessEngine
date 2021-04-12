#pragma once

#include "Window.h"

#include <memory>

class GameInstance
{
public:
	void init();

	void run();

private:
	std::unique_ptr<Window> window;
};
