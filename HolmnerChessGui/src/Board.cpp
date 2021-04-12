#include "Board.h"

#include "Drawable.h"
#include "Window.h"
#include "ImageData.h"

#include "ThirdPartyWrappersFactory.h"

#include <cassert>

Board::Board(const ImageData& image)
	: boardDrawable{ThirdPartyWrappersFactory::createDrawable(image)}
{
}

void Board::setNormalizedPosition(const Vec2<float>& normPos, const Window& window)
{
	assert(boardDrawable != nullptr);
	boardDrawable->setNormalizedPosition(normPos, window);
}

void Board::setScale(const Vec2<float>& scale)
{
	assert(boardDrawable != nullptr);
	boardDrawable->setScale(scale);
}

void Board::draw(Window& window)
{
	assert(boardDrawable != nullptr);
	window.draw(*boardDrawable);
}
