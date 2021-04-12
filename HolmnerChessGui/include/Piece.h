#pragma once

#include <memory>

class Drawable;
struct ImageData;

class Piece
{
public:
	Piece() = default;
	~Piece() = default;

	void init(const ImageData& image);

	Drawable& getDrawable();
	const Drawable& getDrawable() const;

	// A piece may be 'none', such that if it is drawn on a board, nothing is drawn.
	// The 'none' flag is cleared upon calling init().
	bool isPieceNone() const { return isNone; }

private:
	bool isNone = true;
	std::unique_ptr<Drawable> drawable;
};
