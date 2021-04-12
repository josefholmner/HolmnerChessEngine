#pragma once

#include "Vec2.h"

#include <cstdint>

class Window;

class Drawable
{
public:
	// normPos goes from 0.0-1.0 to end up in the drawing section of the window.
	virtual void setNormalizedPosition(const Vec2<float>& normPos, const Window& window) = 0;
	virtual Vec2<float> getNormalizedPosition(const Window& window) = 0;

	// Size as fraction of the drawing section of the window.
	virtual Vec2<float> getNormalizedSize(const Window& window) const = 0;

	virtual void setScale(const Vec2<float>& scale) = 0;
	virtual Vec2<float> getScale() const = 0;
};
