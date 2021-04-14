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

	setUpPieces(window, scale);
}

void Board::draw(Window& window, const Vec2<int32_t>& mousePos)
{
	assert(boardDrawable != nullptr);

	// Draw board.
	window.draw(*boardDrawable);

	// Draw pieces.
	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		pieces[sq].draw(window, mousePos);
	}
}

void Board::registerMouseDown(const Vec2<int32_t>& mousePos, const Window& window)
{
	const auto normalizedPos = window.pixelToNormalizedPosition(mousePos);
	const Square sq = getFromNormalizedPosition(normalizedPos, window);
	if (sq == squares::none || pieces[sq].getType() == Piece::Type::None)
	{
		return;
	}

	dragStartSquare = sq;
	pieces[sq].setIsMouseDragged(true);
}

void Board::registerMouseRelease(const Vec2<int32_t>& mousePos, const Window& window)
{
	if (dragStartSquare == squares::none)
	{
		return;
	}

	// Reset the dragged piece.
	pieces[dragStartSquare].setIsMouseDragged(false);
	const auto dragStartPos = getSquareNormalizedPosition(dragStartSquare, window);
	pieces[dragStartSquare].setNormalizedPosition(dragStartPos, window);
	dragStartSquare = squares::none;
}

void Board::setUpPieces(const Window& window, const Vec2<float>& scale)
{
	pieces[squares::a1].init(Piece::Type::WRook, scale);
	pieces[squares::b1].init(Piece::Type::WKnight, scale);
	pieces[squares::c1].init(Piece::Type::WBishop, scale);
	pieces[squares::d1].init(Piece::Type::WQueen, scale);
	pieces[squares::e1].init(Piece::Type::WKing, scale);
	pieces[squares::f1].init(Piece::Type::WBishop, scale);
	pieces[squares::g1].init(Piece::Type::WKnight, scale);
	pieces[squares::h1].init(Piece::Type::WRook, scale);

	for (Square s = squares::a2; s <= squares::h2; s++)
	{
		pieces[s].init(Piece::Type::WPawn, scale);
	}
	
	pieces[squares::a8].init(Piece::Type::BRook, scale);
	pieces[squares::b8].init(Piece::Type::BKnight, scale);
	pieces[squares::c8].init(Piece::Type::BBishop, scale);
	pieces[squares::d8].init(Piece::Type::BQueen, scale);
	pieces[squares::e8].init(Piece::Type::BKing, scale);
	pieces[squares::f8].init(Piece::Type::BBishop, scale);
	pieces[squares::g8].init(Piece::Type::BKnight, scale);
	pieces[squares::h8].init(Piece::Type::BRook, scale);

	for (Square s = squares::a7; s <= squares::h7; s++)
	{
		pieces[s].init(Piece::Type::BPawn, scale);
	}

	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		placePieceAtSquare(pieces[sq], sq, window);
	}
}

void Board::placePieceAtSquare(Piece& piece, Square sq, const Window& window)
{
	if (piece.getType() == Piece::Type::None)
	{
		return;
	}
	
	const Vec2<float> squareNormalizedPos = getSquareNormalizedPosition(sq, window);
	piece.setNormalizedPosition(squareNormalizedPos, window);
}

Square Board::getFromNormalizedPosition(const Vec2<float>& normPos, const Window& window) const
{
	const Vec2<float> topLeftCorner = boardDrawable->getNormalizedPosition(window);
	if (normPos.x() <= topLeftCorner.x() || normPos.y() <= topLeftCorner.y())
	{
		// Position outside the board.
		return squares::none;
	}

	const Vec2<float> boardNormalizedSize = boardDrawable->getNormalizedSize(window);
	if (normPos.x() >= topLeftCorner.x() + boardNormalizedSize.x() ||
		normPos.y() >= topLeftCorner.y() + boardNormalizedSize.y())
	{
		// Position outside the board.
		return squares::none;
	}

	const Vec2<float> posOnBoard((normPos.x() - topLeftCorner.x()) / boardNormalizedSize.x(),
		(normPos.y() - topLeftCorner.y()) / boardNormalizedSize.y());
	const int32_t file = static_cast<int32_t>(posOnBoard.x() * 8);
	const int32_t reversedRank = 7 - static_cast<int32_t>(posOnBoard.y() * 8);

	return file + reversedRank * 8;
}

Vec2<float> Board::getSquareNormalizedPosition(Square sq, const Window& window) const
{
	const Vec2<float> topLeftCorner = boardDrawable->getNormalizedPosition(window);
	const Vec2<float> boardNormalizedSize = boardDrawable->getNormalizedSize(window);
	const Vec2<float> squareNormalizedSize(
		boardNormalizedSize.x() / 8.f, boardNormalizedSize.y() / 8.f);

	const int32_t reverseRank = 7 - (sq / 8);
	const int32_t file = sq % 8;

	return Vec2<float>(topLeftCorner.x() + squareNormalizedSize.x() * file,
		topLeftCorner.y() + squareNormalizedSize.y() * reverseRank);
}
