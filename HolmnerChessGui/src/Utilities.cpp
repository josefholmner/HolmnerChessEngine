#include "Utilities.h"

void Utilities::Log(Logger::Type type, const std::string& msg)
{
	Logger::Log(type, "Engine", msg);
}
