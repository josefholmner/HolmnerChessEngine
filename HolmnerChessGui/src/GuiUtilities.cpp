#include "GuiUtilities.h"

#include "Common/Logger.h"

void GuiUtilities::log(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Log, "GUI", msg);
}

void GuiUtilities::logW(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Warning, "GUI", msg);
}

void GuiUtilities::logE(const std::string& msg)
{
	hceCommon::Logger::log(hceCommon::Logger::Type::Error, "GUI", msg);
}
