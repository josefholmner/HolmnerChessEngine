#include "PrivateInclude/EngineUtilities.h"

#include "Common\Logger.h"

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