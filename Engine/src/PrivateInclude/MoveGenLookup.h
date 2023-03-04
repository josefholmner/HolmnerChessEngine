#pragma once

#include "PiecesSquaresAndMoves.h"

#include <array>

struct PawnAttackSquares
{
  SmallVal num = 0;
  std::array<Square, 2> squares = {};
};

class MoveGenLookup
{
public:
  static const MoveGenLookup& Get();

  const PawnAttackSquares& GetPawnAttackSquaresWhite(Square sq) const
  {
    return pawnAttackSquaresWhite[sq];
  }

  const PawnAttackSquares& GetPawnAttackSquaresBlack(Square sq) const
  {
    return pawnAttackSquaresBlack[sq];
  }

private:
  MoveGenLookup();
  MoveGenLookup(const MoveGenLookup&) = delete;
  MoveGenLookup operator=(const MoveGenLookup&) = delete;

  void Init();

  std::array<File, NUM_SQUARES> squareToFile;
  std::array<Rank, NUM_SQUARES> squareToRank;

  std::array<PawnAttackSquares, NUM_SQUARES> pawnAttackSquaresWhite;
  std::array<PawnAttackSquares, NUM_SQUARES> pawnAttackSquaresBlack;
};