#pragma once

#include "Engine/EngineAPIReturnDefinitions.h"
#include "PrivateInclude/PiecesSquaresAndMoves.h"

#include <optional>
#include <array>

class Board;

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 26495) 
#endif

struct MoveCollection
{
  SmallVal num = 0;
  std::array<Move, MAX_NUM_MOVES> moves;
};

class Engine
{
public:
  std::optional<size_t> getNumLegalMoves(const std::string& FEN, uint8_t depth) const;

  // @todo compare performance using std::vector. Remove disabled warning above if changed.
  MoveCollection getLegalMoves(const Board& board) const;
private:
};

#ifdef _WIN32
#pragma warning( pop )
#endif