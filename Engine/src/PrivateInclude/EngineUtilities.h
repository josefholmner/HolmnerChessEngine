#pragma once

#include "PrivateInclude/PiecesSquaresAndMoves.h"

#include <string>

class EngineUtilities
{
public:
	static void log(const std::string& msg);
	static void logW(const std::string& msg);
	static void logE(const std::string& msg);

	static void printBitBoard(BitBoard bitBoard, const std::string& name);
	static void printSquareOnBoard(Square sq, const std::string& name);
};