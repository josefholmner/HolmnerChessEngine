#pragma once

#include "Vec2.h"

#include <cstdint>

class Window;

class Drawable
{
public:
	// relPos goes from 0.0-1.0 to end up in the drawn window.
	virtual void setRelativePosition(const Vec2<float>& relPos, const Window& window) = 0;

	virtual void setScale(const Vec2<float>& scale) = 0;
	virtual Vec2<float> getScale() const = 0;
};
