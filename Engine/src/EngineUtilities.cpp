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

std::string EngineUtilities::pieceToStr(Piece piece)
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
			logE("Piece to string, unknown piece: " + std::to_string(piece));
			return "";
	}
}
