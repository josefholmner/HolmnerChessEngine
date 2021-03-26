#pragma once

#include "PiecesAndSquares.h"

#include <string>
#include <array>
#include <unordered_map>
#include <optional>

struct Move;

class BoardState
{
public:
	bool initFromFEN(const std::string& FEN);

	void printBoard() const;

	void makeMove(const Move& move);
	void unmakeMove(const Move& move);

	// Note: these ONLY updates the pieces on the board, not e.g. en passant created squares,
	// castling availability or even turn. Use with caution.
	void makeMovePiecesOnly(const Move& move);
	void unmakeMovePiecesOnly(const Move& move);

	bool isValid() const;

	pieces::Color getTurn() const
	{
		return turn;
	}

	const std::array<Piece, squares::num>& getPieces() const
	{
		return pieces;
	}

	std::array<Piece, squares::num>& getPieces()
	{
		return pieces;
	}

	Piece getPiece(Square sq) const
	{
		return pieces[sq];
	}

	const std::unordered_map<char, bool> getCastleAvailability() const
	{
		return casleAvailability;
	}

	std::unordered_map<char, bool>& getCastleAvailability()
	{
		return casleAvailability;
	}

	Square getEnPassantSquare() const
	{
		return enPassantSquare;
	}

	Square getWhiteKingSquare() const
	{
		return wKingSq;
	}

	void setWhiteKingSquare(Square sq)
	{
		wKingSq = sq;
	}

	Square getBlackKingSquare() const
	{
		return bKingSq;
	}

	void setBlackKingSquare(Square sq)
	{
		bKingSq = sq;
	}

private:
	void makePawnMove(const Move& move);
	void unmakePawnMove(const Move& move);

	void makeKingMove(const Move& move);
	void unmakeKingMove(const Move& move);

	void makeWhiteRookMoveIfCastling(const Move& move);
	void makeBlackRookMoveIfCastling(const Move& move);

	void unmakeWhiteRookMoveIfCastling(const Move& move);
	void unmakeBlackRookMoveIfCastling(const Move& move);

	// For any piece that is not a pawn, king or rook.
	void makeNonSpecializedMove(const Move& move);
	void unmakeNonSpecializedMove(const Move& move);

	std::array<Piece, squares::num> pieces;
	std::unordered_map<char, bool> casleAvailability;
	pieces::Color turn;
	Square enPassantSquare = squares::none;
	Square wKingSq = squares::none;
	Square bKingSq = squares::none;
};
