#pragma once

#include "ResourceData.h"

class Resources
{
public:
	static const ResourceData getMenuBackgroundImg();
	static const ResourceData getMenuButtonHardImg();
	static const ResourceData getMenuButtonMediumImg();
	static const ResourceData getMenuButtonEasyImg();
	static const ResourceData getMenuButtonSillyImg();
	static const ResourceData getMenuButtonSideWhiteImg();
	static const ResourceData getMenuButtonSideBlackImg();

	static const ResourceData getBoardImg();

	static const ResourceData getWhiteKingImg();
	static const ResourceData getWhiteQueenImg();
	static const ResourceData getWhiteRookImg();
	static const ResourceData getWhiteBishopImg();
	static const ResourceData getWhiteKnightImg();
	static const ResourceData getWhitePawnImg();

	static const ResourceData getBlackKingImg();
	static const ResourceData getBlackQueenImg();
	static const ResourceData getBlackRookImg();
	static const ResourceData getBlackBishopImg();
	static const ResourceData getBlackKnightImg();
	static const ResourceData getBlackPawnImg();

	static const ResourceData getBoardEdgeWhiteImg();
	static const ResourceData getBoardEdgeBlackImg();
	static const ResourceData getWhiteToPlayLitImg();
	static const ResourceData getWhiteToPlayUnlitImg();
	static const ResourceData getBlackToPlayLitImg();
	static const ResourceData getBlackToPlayUnlitImg();

	static const ResourceData getWhiteWinsImg();
	static const ResourceData getBlackWinsImg();
	static const ResourceData getDrawImg();

	static const ResourceData getDefaultFont();
};
