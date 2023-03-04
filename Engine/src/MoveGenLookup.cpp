#include "PrivateInclude/MoveGenLookup.h"

#include <cassert>

namespace MoveGenLookup_helpers
{
  void setSquareToRank(std::array<Rank, NUM_SQUARES>& squareToRank)
  {
    for (Square sq = A1; sq < NUM_SQUARES; sq++)
    {
      squareToRank[sq] = sq / NUM_RANKS;
    }
  }

  void setSquareToFile(std::array<File, NUM_SQUARES>& squareToFile)
  {
    for (Square sq = A1; sq < NUM_SQUARES; sq++)
    {
      squareToFile[sq] = sq % NUM_FILES;
    }
  }

  void setPawnAttackSquare(
    const std::array<File, NUM_SQUARES>& squareToFile, Square sq, Side side, PawnAttackSquares& attackSqs)
  {
    attackSqs.squares.fill(NO_SQUARE);

    const Square attack1 = sq + side == WHITE ? 7 : -9;
    const Square attack2 = sq + side == WHITE ? 9 : -7;

    if (attack1 < 0 || attack2 < 0)
    {
      assert(false);
      return;
    }

    if (squareToFile[sq] == squareToFile[attack1] + 1)
      attackSqs.squares[attackSqs.num++] = attack1;

    if (squareToFile[sq] + 1 == squareToFile[attack2])
      attackSqs.squares[attackSqs.num++] = attack2;
  }
}

const MoveGenLookup& MoveGenLookup::Get()
{
  static MoveGenLookup instance;
  return instance;
}

MoveGenLookup::MoveGenLookup()
{
  Init();
}

void MoveGenLookup::Init()
{
  using namespace MoveGenLookup_helpers;

  setSquareToFile(squareToFile);
  setSquareToRank(squareToRank);
  
  for (Square sq = A1; sq < NUM_SQUARES; sq++)
  {
    setPawnAttackSquare(squareToFile, sq, WHITE, pawnAttackSquaresWhite[sq]);
    setPawnAttackSquare(squareToFile, sq, BLACK, pawnAttackSquaresBlack[sq]);
  }
}
