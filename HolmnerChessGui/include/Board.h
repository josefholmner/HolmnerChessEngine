#pragma once

#include "Vec2.h"
#include "Squares.h"
#include "Piece.h"
#include "CapturedPiece.h"
#include "Text.h"
#include "StatesAndEvents.h"
#include "Engine/EngineAPIReturnDefinitions.h"

#include <memory>
#include <array>
#include <vector>
#include <cstdint>
#include <optional>
#include <string>

class Drawable;
class Window;
struct ResourceData;

struct UserMove
{
	std::string fromSquare;
	std::string toSquare;
};

class Board
{
public:
	Board(const ResourceData& image);

	void init(const Vec2<float>& normPos, const Vec2<float>& inScale,
		statesAndEvents::PlayingSide side, const Window& window);

	void draw(Window& window, const Vec2<int32_t>& mousePos, statesAndEvents::PlayingSide turn);

	void makeMove(const hceEngine::ChessMove& move, const Window& window);

	void registerMouseDown(const Vec2<int32_t>& mousePos, const Window& window);
	std::optional<UserMove> registerMouseRelease(const Vec2<int32_t>& mousePos, const Window& window);

	std::string getFEN() const { return FEN; }

	statesAndEvents::PlayingSide getUserSide() const { return userSide; }

	void setPlayInfoText(const std::string& str);

	int32_t getMoveCount() const { return moveCount; }

private:
	void setUpPieces(const Window& window);
	void initSideToPlayImg(const Window& window);

	void placePieceAtSquare(Piece& piece, Square sq, const Window& window);

	Vec2<float> getSquareNormalizedPosition(Square sq, const Window& window) const;
	Square getFromNormalizedPosition(const Vec2<float>& normPos,
		const Window& window) const;

	void makeCastlingMove(const hceEngine::ChessMove& move, const Window& window);
	void makePawnPromotionMove(const hceEngine::ChessMove& move, const Window& window);
	void makeEnPassantCaptureMove(const hceEngine::ChessMove& move, const Window& window);
	void makeRegularMove(const hceEngine::ChessMove& move, const Window& window);

	std::string moveToStr(const hceEngine::ChessMove& move) const;

	void appendCapturedPiece(const hceEngine::ChessMove& move, const Window& window);

	void drawSideToPlay(Window& window, statesAndEvents::PlayingSide turn);

	statesAndEvents::PlayingSide userSide;
	Vec2<float> scale;
	int32_t moveCount = 0;
	std::unique_ptr<Drawable> boardDrawable;
	std::unique_ptr<Drawable> whiteToPlayLit;
	std::unique_ptr<Drawable> whiteToPlayUnlit;
	std::unique_ptr<Drawable> blackToPlayLit;
	std::unique_ptr<Drawable> blackToPlayUnlit;
	std::array<Piece, squares::num> pieces;
	Square dragStartSquare = squares::none;
	std::string FEN;
	std::unique_ptr<Text> playInfoText;
	std::unique_ptr<Text> moveListText;
	std::vector<std::string> moveList;
	std::vector<CapturedPiece> capturedBlackPieces;
	std::vector<CapturedPiece> capturedWhitePieces;
};
