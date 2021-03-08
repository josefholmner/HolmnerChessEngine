#pragma once

#include <string>

namespace hceCommon
{
	class Logger
	{
	public:

		enum class Type
		{
			Log,
			Warning,
			Error
		};

		static void log(Type type, const std::string& sender, const std::string& msg);
	};
}
