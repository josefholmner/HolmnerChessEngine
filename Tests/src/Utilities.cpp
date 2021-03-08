#include "Utilities.h"

#include "Common/Logger.h"

void Utilities::Log(const std::string& msg)
{
	hceCommon::Logger::Log(hceCommon::Logger::Type::Log, "Tests", msg);
}

void Utilities::LogW(const std::string& msg)
{
	hceCommon::Logger::Log(hceCommon::Logger::Type::Warning, "Tests", msg);
}

void Utilities::LogE(const std::string& msg)
{
	hceCommon::Logger::Log(hceCommon::Logger::Type::Error, "Tests", msg);
}
