#pragma once

#include "Vec2.h"

#include <cstdint>

class Drawable
{
	virtual void setPosition(const Vec2<uint32_t>& pos) = 0;
};
