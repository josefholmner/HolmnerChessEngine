#pragma once

#include "Vec2.h"

#include <memory>

class Drawable;
class Window;
struct ImageData;

class Board
{
public:
	Board(const ImageData& image);

	// normPos goes from 0.0-1.0 to end up in the drawing section of the window.
	void setNormalizedPosition(const Vec2<float>& normPos, const Window& window);
	void setScale(const Vec2<float>& scale);

	void draw(Window& window);

private:
	std::unique_ptr<Drawable> boardDrawable;
};
