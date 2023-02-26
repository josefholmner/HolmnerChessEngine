#include "PrivateInclude/Engine.h"

#include "PrivateInclude\Board.h"

std::optional<size_t> Engine::getNumLegalMoves(const std::string& FEN, uint8_t depth) const
{
  Board board;
  board.init(FEN);

  return {};
}
