#include "CapturedPiece.h"

#include "GuiUtilities.h"
#include "Drawable.h"

#include "ThirdPartyWrappersFactory.h"

CapturedPiece::CapturedPiece(const CapturedPiece& other)
{
	init(other.getType(), other.getScale());
}

void CapturedPiece::init(Type type, const Vec2<float>& inScale)
{
	Piece::init(type, inScale);

	// Set orderValue used for sorting.
	switch (type)
	{
		case Piece::Type::None:
			GuiUtilities::logE("CapturedPiece may not be of type None.");
			break;
		case Piece::Type::WKing:
			GuiUtilities::logE("CapturedPiece may not be of type WKing.");
			break;
		case Piece::Type::WQueen:
		case Piece::Type::BQueen:
			orderValue = 0;
			break;
		case Piece::Type::WRook:
		case Piece::Type::BRook:
			orderValue = 1;
			break;
		case Piece::Type::WBishop:
		case Piece::Type::BBishop:
			orderValue = 2;
			break;
		case Piece::Type::WKnight:
		case Piece::Type::BKnight:
			orderValue = 3;
			break;
		case Piece::Type::WPawn:
		case Piece::Type::BPawn:
			orderValue = 4;
			break;
		case Piece::Type::BKing:
			GuiUtilities::logE("CapturedPiece may not be of type BKing.");
			break;
		default:
			GuiUtilities::logE("CapturedPiece set to unknown type.");
			break;
	}
}

bool CapturedPiece::operator<(const CapturedPiece& other) const
{
	return orderValue < other.getOrderValue();
}

CapturedPiece& CapturedPiece::operator=(const CapturedPiece& other)
{
	this->init(other.getType(), other.getScale());
	return *this;
}
