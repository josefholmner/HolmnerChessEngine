#pragma once

#include <string>
#include <optional>

class EngineAPI
{
public:

    std::optional<int32_t> getNumLegalMoves(const std::string& FEN, int32_t depth) const;
};