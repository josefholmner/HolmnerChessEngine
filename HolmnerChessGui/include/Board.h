#pragma once

#include "Vec2.h"
#include "Squares.h"
#include "Piece.h"
#include "StatesAndEvents.h"
#include "Engine/EngineAPIReturnDefinitions.h"

#include <memory>
#include <array>
#include <cstdint>
#include <optional>
#include <string>

class Drawable;
class Window;
struct ImageData;

struct UserMove
{
	std::string fromSquare;
	std::string toSquare;
};

class Board
{
public:
	Board(const ImageData& image);

	void init(const Vec2<float>& normPos, const Vec2<float>& scale,
		statesAndEvents::PlayingSide side, const Window& window);

	void draw(Window& window, const Vec2<int32_t>& mousePos);

	void makeMove(const hceEngine::ChessMove& move, const Window& window);

	void registerMouseDown(const Vec2<int32_t>& mousePos, const Window& window);
	std::optional<UserMove> registerMouseRelease(const Vec2<int32_t>& mousePos, const Window& window);

	std::string getFEN() const { return FEN; }

	statesAndEvents::PlayingSide getUserSide() const { return userSide; }

private:
	void setUpPieces(const Window& window, const Vec2<float>& scale);

	void placePieceAtSquare(Piece& piece, Square sq, const Window& window);

	Vec2<float> getSquareNormalizedPosition(Square sq, const Window& window) const;
	Square getFromNormalizedPosition(const Vec2<float>& normPos,
		const Window& window) const;

	void makeCastlingMove(const hceEngine::ChessMove& move, const Window& window);
	void makePawnPromotionMove(const hceEngine::ChessMove& move, const Window& window);
	void makeEnPassantCaptureMove(const hceEngine::ChessMove& move, const Window& window);
	void makeRegularMove(const hceEngine::ChessMove& move, const Window& window);

	statesAndEvents::PlayingSide userSide;
	std::unique_ptr<Drawable> boardDrawable;
	std::array<Piece, squares::num> pieces;
	Square dragStartSquare = squares::none;
	std::string FEN;
};
