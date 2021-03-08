#include "PrivateInclude/Utilities.h"

#include "Common/Logger.h"

void Utilities::Log(const std::string& msg)
{
	hceCommon::Logger::Log(hceCommon::Logger::Type::Log, "Engine", msg);
}

void Utilities::LogW(const std::string& msg)
{
	hceCommon::Logger::Log(hceCommon::Logger::Type::Warning, "Engine", msg);
}

void Utilities::LogE(const std::string& msg)
{
	hceCommon::Logger::Log(hceCommon::Logger::Type::Error, "Engine", msg);
}
