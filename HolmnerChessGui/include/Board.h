#pragma once

#include "Vec2.h"
#include "Squares.h"
#include "Piece.h"

#include <memory>
#include <array>

class Drawable;
class Window;
struct ImageData;

class Board
{
public:
	Board(const ImageData& image);

	void init(const Vec2<float>& normPos, const Vec2<float>& scale, const Window& window);

	void draw(Window& window);

private:
	void setPiecesStartPosition(const Window& window);

	void placePieceAtSquare(Piece& piece, Square sq, const Window& window);

	Vec2<float> getSquareNormalizedPosition(Square sq, const Window& window);

	std::unique_ptr<Drawable> boardDrawable;
	std::array<Piece, squares::num> pieces;
};
