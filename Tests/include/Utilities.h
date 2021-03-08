#pragma once

#include <string>

class Utilities
{
public:
	static void Log(const std::string& msg);
	static void LogW(const std::string& msg);
	static void LogE(const std::string& msg);
};
