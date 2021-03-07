#pragma once

#include <cstdint>

class Window
{
public:
	virtual void open(uint32_t width = 1440, uint32_t height = 900) = 0;

	virtual bool isOpen() = 0;
};
