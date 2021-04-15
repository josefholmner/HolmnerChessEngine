#include "Squares.h"

std::string squares::toString(Square sq)
{
	if (sq == squares::none)
	{
		return "";
	}

	const char fileCh = 'a' + sq % 8;
	const char rankCh = '1' + sq / 8;
	return std::string({ fileCh, rankCh });
}

Square squares::fromString(const std::string& sq)

{
	if (sq.size() != 2)
	{
		return squares::none;
	}

	const int32_t file = sq[0] - 'a';
	const int32_t rank = sq[1] - '1';
	return file + rank * 8;
}
