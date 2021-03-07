#pragma once

#include "Window.h"

#include <memory>

class GameInstance
{
public:
	void startNew();

private:
	void run();

	std::unique_ptr<Window> window;
};
