#pragma once

#include "Vec2.h"
#include "Squares.h"
#include "Piece.h"

#include <memory>
#include <array>
#include <cstdint>
#include <optional>

class Drawable;
class Window;
struct ImageData;

class Board
{
public:
	Board(const ImageData& image);

	void init(const Vec2<float>& normPos, const Vec2<float>& scale,
		 const Window& window);

	void draw(Window& window, const Vec2<int32_t>& mousePos);

	void registerMouseDown(const Vec2<int32_t>& mousePos, const Window& window);
	void registerMouseRelease(const Vec2<int32_t>& mousePos, const Window& window);

private:
	void setUpPieces(const Window& window, const Vec2<float>& scale);

	void placePieceAtSquare(Piece& piece, Square sq, const Window& window);

	Vec2<float> getSquareNormalizedPosition(Square sq, const Window& window) const;
	Square getFromNormalizedPosition(const Vec2<float>& normPos,
		const Window& window) const;

	std::unique_ptr<Drawable> boardDrawable;
	std::array<Piece, squares::num> pieces;
	Square dragStartSquare = squares::none;
};
