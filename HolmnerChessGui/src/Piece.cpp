#include "Piece.h"

#include "Drawable.h"
#include "ImageData.h"
#include "Window.h"
#include "GuiUtilities.h"
#include "Resources.h"
#include "Vec2.h"

#include "ThirdPartyWrappersFactory.h"

#include <cassert>


void Piece::init(Piece::Type type, const Vec2<float>& scale)
{
	setType(type);

	if (drawable != nullptr)
	{
		drawable->setScale(scale);
	}
}

void Piece::draw(Window& window, const Vec2<int32_t>& mousePos)
{
	if (type == Type::None || drawable == nullptr)
	{
		return;
	}

	if (isMouseDragged)
	{
		// The piece follows the mouse cursor.
		const auto normPos = window.pixelToNormalizedPosition(mousePos);

		// Compensate for the origo of the drawable beeing in the top left corner.
		const auto drawableSize = drawable->getNormalizedSize(window);
		const auto finalPos = Vec2<float>(normPos.x() - drawableSize.x() / 2.f,
			normPos.y() - drawableSize.y() / 2.f);
		setNormalizedPosition(finalPos, window);
	}

	window.draw(*drawable);
}

void Piece::setType(Type inType)
{
	type = inType;

	// Update drawable according to type.
	switch (type)
	{
	case Piece::Type::None:
		break;
	case Piece::Type::WKing:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getWhiteKingImg());
		break;
	case Piece::Type::WQueen:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getWhiteQueenImg());
		break;
	case Piece::Type::WRook:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getWhiteRookImg());
		break;
	case Piece::Type::WBishop:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getWhiteBishopImg());
		break;
	case Piece::Type::WKnight:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getWhiteKnightImg());
		break;
	case Piece::Type::WPawn:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getWhitePawnImg());
		break;
	case Piece::Type::BKing:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getBlackKingImg());
		break;
	case Piece::Type::BQueen:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getBlackQueenImg());
		break;
	case Piece::Type::BRook:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getBlackRookImg());
		break;
	case Piece::Type::BBishop:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getBlackBishopImg());
		break;
	case Piece::Type::BKnight:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getBlackKnightImg());
		break;
	case Piece::Type::BPawn:
		drawable = ThirdPartyWrappersFactory::createDrawable(Resources::getBlackPawnImg());
		break;
	default:
		GuiUtilities::logE("Unknown piece type passed to Piece::init().");
		return;
	}
}

void Piece::setNormalizedPosition(const Vec2<float>& normPos, const Window& window)
{
	if (drawable == nullptr)
	{
		return;
	}

	drawable->setNormalizedPosition(normPos, window);
}

Vec2<float> Piece::getNormalizedPosition(const Window& window) const
{
	if (drawable == nullptr)
	{
		return Vec2<float>();
	}

	return drawable->getNormalizedPosition(window);
}

void Piece::setIsMouseDragged(bool inIsMouseDragged)
{
	assert(type != Type::None);
	isMouseDragged = inIsMouseDragged;
}
