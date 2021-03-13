#include "PrivateInclude/EngineUtilities.h"

#include "Common/Logger.h"

void EngineUtilities::log(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Log, "Engine", msg);
}

void EngineUtilities::logW(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Warning, "Engine", msg);
}

void EngineUtilities::logE(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Error, "Engine", msg);
}

bool EngineUtilities::isBlack(Piece piece)
{
	switch (piece)
	{
	case pieces::bK:
	case pieces::bQ:
	case pieces::bR:
	case pieces::bB:
	case pieces::bN:
	case pieces::bP:
		return true;
	default:
		return false;
	}
}

bool EngineUtilities::isWhite(Piece piece)
{
	switch (piece)
	{
	case pieces::wK:
	case pieces::wQ:
	case pieces::wR:
	case pieces::wB:
	case pieces::wN:
	case pieces::wP:
		return true;
	default:
		return false;
	}
}

bool EngineUtilities::isNonNonePiece(Piece piece)
{
	return isBlack(piece) || isWhite(piece);
}

bool EngineUtilities::isValidPiece(Piece piece)
{
	return piece == pieces::none || isBlack(piece) || isWhite(piece);
}

bool EngineUtilities::isNonNoneSquare(Square sq)
{
	if (sq == squares::none)
	{
		return false;
	}

	return isValidSquare(sq);
}

bool EngineUtilities::isValidSquare(Square sq)
{
	return sq >= squares::none && sq <= squares::h8;
}
