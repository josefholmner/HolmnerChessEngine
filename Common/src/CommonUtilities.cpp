#include "Common/CommonUtilities.h"

#include <sstream>

std::vector<std::string> CommonUtilities::splitString(const std::string& str, char delimiter)
{
	std::stringstream stream(str);
	std::string segment;
	std::vector<std::string> vec;

	while (std::getline(stream, segment, delimiter))
	{
		vec.push_back(segment);
	}

	return vec;
}
