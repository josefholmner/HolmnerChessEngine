#pragma once

#include <string>

class EngineUtilities
{
public:
	static void log(const std::string& msg);
	static void logW(const std::string& msg);
	static void logE(const std::string& msg);
};