#include "PrivateInclude/PiecesAndSquares.h"

#include "PrivateInclude/EngineUtilities.h"


std::string pieces::pieceToStr(Piece piece)
{
	switch (piece)
	{
	case pieces::none:
		return "  ";
	case pieces::wK:
		return "wK";
	case pieces::wQ:
		return "wQ";
	case pieces::wR:
		return "wR";
	case pieces::wB:
		return "wB";
	case pieces::wN:
		return "wN";
	case pieces::wP:
		return "wP";
	case pieces::bK:
		return "bK";
	case pieces::bQ:
		return "bQ";
	case pieces::bR:
		return "bR";
	case pieces::bB:
		return "bB";
	case pieces::bN:
		return "bN";
	case pieces::bP:
		return "bP";
	default:
		EngineUtilities::logE("Piece to string, unknown piece: " + std::to_string(piece));
		return "";
	}
}

Rank ranks::toRank(Square sq)
{
	return sq / 8;
}

char ranks::rankToChar(Rank rank)
{
	return '1' + rank;
}

File files::toFile(Square sq)
{
	return sq % 8;
}

char files::fileToChar(File file)
{
	return 'A' + file;
}

std::string squares::squareToStr(Square sq)
{
	const char fileChar = files::fileToChar(files::toFile(sq));
	const char rankChar = ranks::rankToChar(ranks::toRank(sq));
	return  std::string({ fileChar, rankChar });
}
