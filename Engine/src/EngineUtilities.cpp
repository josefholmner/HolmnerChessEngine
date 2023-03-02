#include "PrivateInclude/EngineUtilities.h"

#include "Common\Logger.h"
#include "PrivateInclude\EngineUtilities.h"

#include <iostream>

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

void EngineUtilities::printBitBoard(BitBoard bitBoard, const std::string& name)
{
	std::cout << "------ " << name << " ------\n";

	for (Rank r = NUM_RANKS - 1; r != static_cast<Rank>(-1); r--)
	{
		std::cout << "        ";
		for (File f = 0; f < NUM_FILES; f++)
		{
			const bool IsOccupied = bitBoard & (BIT << (f + r * NUM_FILES));
			std::cout << IsOccupied;
		}

		std::cout << "\n";
	}
	
	std::cout << "-------------------------\n";
}

void EngineUtilities::printSquareOnBoard(Square sq, const std::string& name)
{
	std::cout << "------ " << name << " ------\n";

	for (Rank r = NUM_RANKS - 1; r != static_cast<Rank>(-1); r--)
	{
		std::cout << "        ";
		for (File f = 0; f < NUM_FILES; f++)
		{
			std::cout << (sq == r * NUM_FILES + f);
		}

		std::cout << "\n";
	}

	std::cout << "-------------------------\n";
}
