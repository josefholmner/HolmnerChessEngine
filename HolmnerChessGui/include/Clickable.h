#pragma once

#include "Vec2.h"
#include "Drawable.h"
#include "Window.h"

class Clickable
{
public:
	virtual bool wasClicked(const Vec2<int32_t>& mousePos, const Window& window) = 0;

	virtual Drawable& getDrawable() = 0;
};
