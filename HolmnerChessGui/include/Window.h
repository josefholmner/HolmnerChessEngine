#pragma once

#include "StatesAndEvents.h"
#include "Vec2.h"
#include "Drawable.h"

#include <cstdint>

class Text;

class Window
{
public:
	static constexpr uint32_t defaultWidth = 1440;
	static constexpr uint32_t defaultHeight = 900;

	virtual void open(const Vec2<uint32_t>& size = { defaultWidth, defaultHeight }) = 0;
	virtual void close() = 0;
	virtual bool isOpen() const = 0;

	virtual Vec2<uint32_t> getSize() const = 0;
	
	virtual void draw(const Drawable& drawable) = 0;
	virtual void draw(const Text& text) = 0;
	virtual void clear() = 0;
	virtual void display() = 0;

	virtual Vec2<float> pixelToNormalizedPosition(const Vec2<int32_t>& pixel) const = 0;

	virtual Vec2<int32_t> getMousePos() const = 0;

	virtual statesAndEvents::Event pollEvent() = 0;
};
