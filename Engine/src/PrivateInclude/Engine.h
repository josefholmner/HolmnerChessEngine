#pragma once

#include "Engine/EngineAPIReturnDefinitions.h"

#include <optional>

class Engine
{
public:
  std::optional<size_t> getNumLegalMoves(const std::string& FEN, uint8_t depth) const;
private:
};
