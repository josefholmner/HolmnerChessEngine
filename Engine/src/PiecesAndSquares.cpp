#include "PrivateInclude/PiecesAndSquares.h"

#include "PrivateInclude/EngineUtilities.h"


std::string pieces::pieceToStr(Piece piece)
{
	switch (piece)
	{
	case pieces::none:
		return " ";
	case pieces::wK:
		return "K";
	case pieces::wQ:
		return "Q";
	case pieces::wR:
		return "R";
	case pieces::wB:
		return "B";
	case pieces::wN:
		return "N";
	case pieces::wP:
		return "P";
	case pieces::bK:
		return "k";
	case pieces::bQ:
		return "q";
	case pieces::bR:
		return "r";
	case pieces::bB:
		return "b";
	case pieces::bN:
		return "n";
	case pieces::bP:
		return "p";
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
	return 'a' + file;
}

std::string squares::squareToStr(Square sq)
{
	const char fileChar = files::fileToChar(files::toFile(sq));
	const char rankChar = ranks::rankToChar(ranks::toRank(sq));
	return  std::string({ fileChar, rankChar });
}
