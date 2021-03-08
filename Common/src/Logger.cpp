#include "Common/Logger.h"

#include <iostream>

void hceCommon:: Logger::Log(Type type, const std::string& sender, const std::string& msg)
{
	switch (type)
	{
		case Type::Log:
			std::cout << "Log " << sender << ": " << msg << std::endl;
			break;
		case Type::Warning:
			std::cerr << "Warning " << sender << ": " << msg << std::endl;
			break;
		case Type::Error:
			std::cerr << "Error " << sender << ": " << msg << std::endl;
			break;
	}
}
