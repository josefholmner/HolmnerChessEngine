#pragma once

#include "PiecesAndSquares.h"

#include <string>

class EngineUtilities
{
public:
	static void log(const std::string& msg);
	static void logW(const std::string& msg);
	static void logE(const std::string& msg);

	static bool isBlack(Piece piece);

	static bool isWhite(Piece piece);

	static bool isNonNonePiece(Piece piece);

	// pieces::none is considered valid here.
	static bool isValidPiece(Piece piece);

	static bool isNonNoneSquare(Square sq);

	// suares::none is considered valid here.
	static bool isValidSquare(Square sq);
};
