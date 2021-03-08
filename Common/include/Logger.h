#pragma once

#include <string>

class Logger
{
public:

	enum class Type
	{
		Log,
		Warning,
		Error
	};

	static void Log(Type type, const std::string& sender, const std::string& msg);
};
