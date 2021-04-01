#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class TestsUtilities
{
public:
	static void log(const std::string& msg);
	static void logW(const std::string& msg);
	static void logE(const std::string& msg);

	static bool isReleaseBuild();

	static const std::vector<std::string>& getFENTests();

	struct FENTestParams
	{
		FENTestParams(const std::string& testStr);

		std::string FEN;

		// Key is depth, value is number of moves.
		std::unordered_map<uint8_t, size_t> numMoves;
	};
};
