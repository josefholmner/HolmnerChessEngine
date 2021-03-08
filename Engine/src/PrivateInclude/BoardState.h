#pragma once

#include "EngineUtilities.h"

#include <string>
#include <vector>
#include <array>
#include <optional>

class BoardState
{
public:
	bool initFromFEN(const std::string& FEN);

	void printBoard() const;

private:
	bool setupPieces(const std::vector<std::string>& ranks);
	std::optional<int32_t> addPiece(char c, int32_t cursor);

	std::array<Piece, squares::num> squares;
};
