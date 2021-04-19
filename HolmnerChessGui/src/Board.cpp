#include "Board.h"

#include "Drawable.h"
#include "Window.h"
#include "ResourceData.h"
#include "Resources.h"
#include "GuiUtilities.h"

#include "ThirdPartyWrappersFactory.h"

#include <cassert>

Board::Board(const ResourceData& image)
	: boardDrawable{ThirdPartyWrappersFactory::createDrawable(image)}
{
}

void Board::init(const Vec2<float>& normPos, const Vec2<float>& scale,
	statesAndEvents::PlayingSide side, const Window& window)
{
	assert(boardDrawable != nullptr);
	boardDrawable->setNormalizedPosition(normPos, window);
	boardDrawable->setScale(scale);

	userSide = side;
	FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // Starting position.
	playInfoText = ThirdPartyWrappersFactory::createText("", Resources::getDefaultFont(), 24);
	setUpPieces(window, scale);
}

void Board::draw(Window& window, const Vec2<int32_t>& mousePos)
{
	assert(boardDrawable != nullptr);
	assert(playInfoText != nullptr);

	// Draw board.
	window.draw(*boardDrawable);

	// Draw pieces.
	Piece* draggedPiece = nullptr;
	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		if (pieces[sq].getIsMouseDragged())
		{
			draggedPiece = &pieces[sq];
			continue; // Dragged piece is drawn last so that it's drawn on top of all other.
		}

		pieces[sq].draw(window, mousePos);
	}

	if (draggedPiece)
	{
		draggedPiece->draw(window, mousePos);
	}

	// Draw text.
	window.draw(*playInfoText);
}

void Board::makeMove(const hceEngine::ChessMove& move, const Window& window)
{
	switch (move.type)
	{
		case hceEngine::MoveType::Silent:
		case hceEngine::MoveType::Capture:
			makeRegularMove(move, window);
			break;		
		case hceEngine::MoveType::Castling:
			makeCastlingMove(move, window);
			break;
		case hceEngine::MoveType::EnPassantCapture:
			makeEnPassantCaptureMove(move, window);
			break;
		case hceEngine::MoveType::PawnPromotionCapture:
		case hceEngine::MoveType::PawnPromotionSilent:
			makePawnPromotionMove(move, window);
			break;
		default:
			GuiUtilities::logE("Invalid move passed to Board::makeMove().");
			return;
	}

	// Lastly, update FEN.
	FEN = move.postMoveFEN;
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

std::optional<UserMove> Board::registerMouseRelease(const Vec2<int32_t>& mousePos, const Window& window)
{
	if (dragStartSquare == squares::none)
	{
		return {};
	}

	const Square startSq = dragStartSquare;

	// Reset the dragged piece.
	pieces[dragStartSquare].setIsMouseDragged(false);
	const auto dragStartPos = getSquareNormalizedPosition(dragStartSquare, window);
	pieces[dragStartSquare].setNormalizedPosition(dragStartPos, window);
	dragStartSquare = squares::none;

	// Get the end square if any.
	const auto normalizedPos = window.pixelToNormalizedPosition(mousePos);
	const Square endSq = getFromNormalizedPosition(normalizedPos, window);
	if (endSq == squares::none)
	{
		return {};
	}

	UserMove userMove;
	userMove.fromSquare = squares::toString(startSq);
	userMove.toSquare = squares::toString(endSq);
	return userMove;
}

void Board::setPlayInfoText(const std::string& str)
{
	if (playInfoText != nullptr)
	{
		playInfoText->setText(str);
	}
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

	for (Square s = squares::a3; s <= squares::h6; s++)
	{
		pieces[s].init(Piece::Type::None, scale);
	}

	for (Square s = squares::a1; s < squares::num; s++)
	{
		placePieceAtSquare(pieces[s], s, window);
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
	using namespace statesAndEvents;

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

	const int32_t file = userSide == PlayingSide::White ?
		(int32_t)(posOnBoard.x() * 8) : 7 - (int32_t)(posOnBoard.x() * 8);
	const int32_t rank = userSide == PlayingSide::White ?
		7 - (int32_t)(posOnBoard.y() * 8) : (int32_t)(posOnBoard.y() * 8);

	return file + rank * 8;
}

void Board::makeCastlingMove(const hceEngine::ChessMove& move, const Window& window)
{
	assert(move.movingPiece == "K" || move.movingPiece == "k");
	using namespace squares;
	const Square fromSq = squares::fromString(move.fromSquare);
	const Square toSq = squares::fromString(move.toSquare);

	// Move the king.
	pieces[toSq].setType(Piece::stringToType(move.movingPiece));
	placePieceAtSquare(pieces[toSq], toSq, window);
	pieces[fromSq].setType(Piece::Type::None);

	// Move the rook.
	if (move.movingPiece == "K")
	{
		if (toSq == c1)
		{
			pieces[d1].setType(Piece::Type::WRook);
			placePieceAtSquare(pieces[d1], d1, window);
			pieces[a1].setType(Piece::Type::None);
		}
		else
		{
			assert(toSq == g1);
			pieces[f1].setType(Piece::Type::WRook);
			placePieceAtSquare(pieces[f1], f1, window);
			pieces[h1].setType(Piece::Type::None);
		}
	}
	else
	{
		assert(move.movingPiece == "k");
		if (toSq == c8)
		{
			pieces[d8].setType(Piece::Type::BRook);
			placePieceAtSquare(pieces[d8], d8, window);
			pieces[a8].setType(Piece::Type::None);
		}
		else
		{
			assert(toSq == g8);
			pieces[f8].setType(Piece::Type::BRook);
			placePieceAtSquare(pieces[f8], f8, window);
			pieces[h8].setType(Piece::Type::None);
		}
	}
}

void Board::makePawnPromotionMove(const hceEngine::ChessMove& move, const Window& window)
{
	assert(move.movingPiece == "P" || move.movingPiece == "p");
	const Square fromSq = squares::fromString(move.fromSquare);
	const Square toSq = squares::fromString(move.toSquare);

	pieces[toSq].setType(Piece::stringToType(move.pawnPromotionPiece));
	placePieceAtSquare(pieces[toSq], toSq, window);

	pieces[fromSq].setType(Piece::Type::None);
}

void Board::makeEnPassantCaptureMove(const hceEngine::ChessMove& move, const Window& window)
{
	assert(move.movingPiece == "P" || move.movingPiece == "p");
	const Square fromSq = squares::fromString(move.fromSquare);
	const Square toSq = squares::fromString(move.toSquare);

	pieces[toSq].setType(Piece::stringToType(move.movingPiece));
	placePieceAtSquare(pieces[toSq], toSq, window);

	pieces[fromSq].setType(Piece::Type::None);
	const Square enemyOffs = move.movingPiece == "P" ? -8 : 8;
	pieces[(size_t)toSq + enemyOffs].setType(Piece::Type::None);
}

void Board::makeRegularMove(const hceEngine::ChessMove& move, const Window& window)
{
	const Square fromSq = squares::fromString(move.fromSquare);
	const Square toSq = squares::fromString(move.toSquare);

	pieces[toSq].setType(Piece::stringToType(move.movingPiece));
	placePieceAtSquare(pieces[toSq], toSq, window);

	pieces[fromSq].setType(Piece::Type::None);
}

Vec2<float> Board::getSquareNormalizedPosition(Square sq, const Window& window) const
{
	using namespace statesAndEvents;

	const Vec2<float> topLeftCorner = boardDrawable->getNormalizedPosition(window);
	const Vec2<float> boardNormalizedSize = boardDrawable->getNormalizedSize(window);
	const Vec2<float> squareNormalizedSize(
		boardNormalizedSize.x() / 8.f, boardNormalizedSize.y() / 8.f);

	const int32_t rank = userSide == PlayingSide::White ?
		7 - (sq / 8) : sq / 8;
	const int32_t file = userSide == PlayingSide::White ?
		sq % 8 : 7 - (sq % 8);

	return Vec2<float>(topLeftCorner.x() + squareNormalizedSize.x() * file,
		topLeftCorner.y() + squareNormalizedSize.y() * rank);
}
