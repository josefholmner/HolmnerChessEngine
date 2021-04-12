#pragma once

#include "ImageData.h"

class Resources
{
public:
	static const ImageData getMenuBackgroundImg();
	static const ImageData getMenuButtonHardImg();
	static const ImageData getMenuButtonMediumImg();
	static const ImageData getMenuButtonEasyImg();
	static const ImageData getMenuButtonSillyImg();
	static const ImageData getMenuButtonSideWhiteImg();
	static const ImageData getMenuButtonSideBlackImg();

	static const ImageData getBoardImg();

	static const ImageData getWhiteKingImg();
	static const ImageData getWhiteQueenImg();
	static const ImageData getWhiteRookImg();
	static const ImageData getWhiteBishopImg();
	static const ImageData getWhiteKnightImg();
	static const ImageData getWhitePawnImg();

	static const ImageData getBlackKingImg();
	static const ImageData getBlackQueenImg();
	static const ImageData getBlackRookImg();
	static const ImageData getBlackBishopImg();
	static const ImageData getBlackKnightImg();
	static const ImageData getBlackPawnImg();
};
