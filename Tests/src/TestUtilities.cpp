#include "TestsUtilities.h"

#include "Common/Logger.h"

void TestsUtilities::log(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Log, "Tests", msg);
}

void TestsUtilities::logW(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Warning, "Tests", msg);
}

void TestsUtilities::logE(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Error, "Tests", msg);
}
