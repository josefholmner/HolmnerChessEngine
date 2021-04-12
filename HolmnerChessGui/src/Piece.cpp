#include "Piece.h"

#include "Drawable.h"
#include "ImageData.h"

#include "ThirdPartyWrappersFactory.h"

#include <cassert>


void Piece::init(const ImageData& image)
{
	drawable = ThirdPartyWrappersFactory::createDrawable(image);
	isNone = false;
}

Drawable& Piece::getDrawable()
{
	assert(drawable != nullptr);
	return *drawable;
}

const Drawable& Piece::getDrawable() const
{
	return getDrawable();
}
