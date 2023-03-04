#pragma once

#include "Engine/EngineAPIReturnDefinitions.h"
#include "PrivateInclude/PiecesSquaresAndMoves.h"

#include <optional>

class Board;

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 26495) 
#endif

class Engine
{
public:
  std::optional<size_t> getNumLegalMoves(const std::string& FEN, uint8_t depth) const;
private:
};
