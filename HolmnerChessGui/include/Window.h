#pragma once

#include "Vec2.h"
#include "Drawable.h"

#include <cstdint>

class Window
{
public:
	virtual void open(const Vec2<uint32_t>& size = { 1440, 900 }) = 0;
	virtual void close() = 0;
	virtual bool isOpen() = 0;
	
	virtual void draw(const Drawable& drawable) = 0;
	virtual void clear() = 0;
	virtual void display() = 0;
};
