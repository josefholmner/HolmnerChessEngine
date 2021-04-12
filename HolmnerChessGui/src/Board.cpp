#include "Board.h"

#include "Drawable.h"
#include "Window.h"
#include "ImageData.h"
#include "Resources.h"

#include "ThirdPartyWrappersFactory.h"

#include <cassert>

Board::Board(const ImageData& image)
	: boardDrawable{ThirdPartyWrappersFactory::createDrawable(image)}
{
}

void Board::init(const Vec2<float>& normPos, const Vec2<float>& scale, const Window& window)
{
	assert(boardDrawable != nullptr);
	boardDrawable->setNormalizedPosition(normPos, window);
	boardDrawable->setScale(scale);

	setPiecesStartPosition(window);

	// Set pieces scale.
	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		if (!pieces[sq].isPieceNone())
		{
			pieces[sq].getDrawable().setScale(scale);
		}
	}
}

void Board::draw(Window& window)
{
	assert(boardDrawable != nullptr);

	// Draw board.
	window.draw(*boardDrawable);

	// Draw pieces.
	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		if (!pieces[sq].isPieceNone())
		{
			window.draw(pieces[sq].getDrawable());
		}
	}
}

void Board::setPiecesStartPosition(const Window& window)
{
	pieces[squares::a1].init(Resources::getWhiteRookImg());
	pieces[squares::b1].init(Resources::getWhiteKnightImg());
	pieces[squares::c1].init(Resources::getWhiteBishopImg());
	pieces[squares::d1].init(Resources::getWhiteQueenImg());
	pieces[squares::e1].init(Resources::getWhiteKingImg());
	pieces[squares::f1].init(Resources::getWhiteBishopImg());
	pieces[squares::g1].init(Resources::getWhiteKnightImg());
	pieces[squares::h1].init(Resources::getWhiteRookImg());

	for (Square s = squares::a2; s <= squares::h2; s++)
	{
		pieces[s].init(Resources::getWhitePawnImg());
	}
	
	pieces[squares::a8].init(Resources::getBlackRookImg());
	pieces[squares::b8].init(Resources::getBlackKnightImg());
	pieces[squares::c8].init(Resources::getBlackBishopImg());
	pieces[squares::d8].init(Resources::getBlackQueenImg());
	pieces[squares::e8].init(Resources::getBlackKingImg());
	pieces[squares::f8].init(Resources::getBlackBishopImg());
	pieces[squares::g8].init(Resources::getBlackKnightImg());
	pieces[squares::h8].init(Resources::getBlackRookImg());

	for (Square s = squares::a7; s <= squares::h7; s++)
	{
		pieces[s].init(Resources::getBlackPawnImg());
	}

	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		placePieceAtSquare(pieces[sq], sq, window);
	}
}

void Board::placePieceAtSquare(Piece& piece, Square sq, const Window& window)
{
	if (piece.isPieceNone())
	{
		return;
	}
	
	const Vec2<float> squareNormalizedPos = getSquareNormalizedPosition(sq, window);
	piece.getDrawable().setNormalizedPosition(squareNormalizedPos, window);
}

Vec2<float> Board::getSquareNormalizedPosition(Square sq, const Window& window)
{
	const Vec2<float> topLeftCorner = boardDrawable->getNormalizedPosition(window);
	const Vec2<float> boardNormalizedSize = boardDrawable->getNormalizedSize(window);
	const Vec2<float> squareNormalizedSize(boardNormalizedSize.x() / 8.f, boardNormalizedSize.y() / 8.f);

	const int32_t reverseRank = 7 - (sq / 8);
	const int32_t file = sq % 8;

	return Vec2<float>(topLeftCorner.x() + squareNormalizedSize.x() * file,
		topLeftCorner.y() + squareNormalizedSize.y() * reverseRank);
}
