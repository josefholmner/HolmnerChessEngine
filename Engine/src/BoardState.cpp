#include "private_include/BoardState.h"

#include <vector>
#include <sstream>

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
	std::vector<std::string> wholeFEN = splitString(FEN, ' ');

	// All FENs should be exactly 6 segments long.
	if (wholeFEN.size() != 6)
	{
		return false;
	}

	std::vector<std::string> rows = splitString(wholeFEN[0], '/');
	if (rows.size() != 8)
	{
		return false;
	}

	return true;
}
