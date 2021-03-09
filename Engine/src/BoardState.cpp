#include "PrivateInclude/BoardState.h"

#include "PrivateInclude/EngineUtilities.h"

#include <sstream>
#include <iostream>

namespace
{
	std::vector<std::string> splitString(const std::string& FEN, char delimiter)
	{
		std::stringstream stream(FEN);
		std::string segment;
		std::vector<std::string> vec;

		while (std::getline(stream, segment, delimiter))
		{
			vec.push_back(segment);
		}

		return vec;
	}
}

bool BoardState::initFromFEN(const std::string& FEN)
{
	const std::vector<std::string> wholeFEN = splitString(FEN, ' ');

	// All FENs should be exactly 6 segments long.
	if (wholeFEN.size() != 6)
	{
		return false;
	}

	const std::vector<std::string> ranks = splitString(wholeFEN[0], '/');
	if (!setupPieces(ranks))
	{
		return false;
	}

	return true;
}

void BoardState::printBoard() const
{
	for (int32_t rank = 7; rank >= 0; rank--)
	{
		for (int32_t file = 0; file < 8; file++)
		{
			const int32_t index = file + rank * 8;
			std::cout << "[" << pieces::pieceToStr(squares[index]) << "] ";
		}

		std::cout << std::endl;
	}
}

bool BoardState::setupPieces(const std::vector<std::string>& ranks)
{
	if (ranks.size() != 8)
	{
		return false;
	}

	int32_t cursor = squares::a1;
	for (int32_t rank = 7; rank >= 0; rank--)
	{
		for (const char& c : ranks[rank])
		{
			if (auto newCursor = addPiece(c, cursor))
			{
				cursor = *newCursor;
			}
			else
			{
				return false;
			}
		}
	}

	if (cursor != squares::num)
	{
		EngineUtilities::logE("Too few squares in FEN.");
		return false;
	}

	return true;
}

std::optional<int32_t> BoardState::addPiece(char c, int32_t cursor)
{
	if (cursor >= squares::num)
	{
		EngineUtilities::logE("Invalid FEN, it represents more than 64 squares.");
		return {};
	}

	switch (c)
	{
		case 'r':
			squares[cursor++] = pieces::bR;
			return cursor;
		case 'n':
			squares[cursor++] = pieces::bN;
			return cursor;
		case 'b':
			squares[cursor++] = pieces::bB;
			return cursor;
		case 'k':
			squares[cursor++] = pieces::bK;
			return cursor;
		case 'q':
			squares[cursor++] = pieces::bQ;
			return cursor;
		case 'p':
			squares[cursor++] = pieces::bP;
			return cursor;
		case 'R':
			squares[cursor++] = pieces::wR;
			return cursor;
		case 'N':
			squares[cursor++] = pieces::wN;
			return cursor;
		case 'B':
			squares[cursor++] = pieces::wB;
			return cursor;
		case 'K':
			squares[cursor++] = pieces::wK;
			return cursor;
		case 'Q':
			squares[cursor++] = pieces::wQ;
			return cursor;
		case 'P':
			squares[cursor++] = pieces::wP;
			return cursor;
		default:
			if (c > '0')
			{
				squares[cursor++] = pieces::none;
				return addPiece(c - 1, cursor);
			}

			return cursor;
	}
}
