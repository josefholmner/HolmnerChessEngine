#pragma once

#include "Vec2.h"

#include <string>
#include <cstdint>


class Window;

class Text
{
public:
	virtual void setText(const std::string& str) = 0;

	virtual void setNormalizedPosition(const Vec2<float>& normPos, const Window& window) = 0;
};
