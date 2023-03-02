#pragma once

#include "PiecesAndSquares.h"

#include <string>
#include <cassert>

struct BitBoards
{
  BitBoard all{ 0 };
  BitBoard pawns{ 0 };
  BitBoard rooks{ 0 };
  BitBoard knights{ 0 };
  BitBoard bishops{ 0 };
  BitBoard queens{ 0 };
  Square king{ 0 };
};

class Board
{
public:
  bool init(const std::string& FEN);

  const BitBoards& getPieces(Side side) const
  {
    assert(side == WHITE || side == BLACK);
    return side == WHITE ? whitePieces : blackPieces;
  }

  void addPawn(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= BIT << sq;
    pieces.pawns |= BIT << sq;
  }

  void addRook(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= BIT << sq;
    pieces.rooks |= BIT << sq;
  }

  void addKnight(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= BIT << sq;
    pieces.knights |= BIT << sq;
  }

  void addBishop(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= BIT << sq;
    pieces.bishops |= BIT << sq;
  }

  void addQueen(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= BIT << sq;
    pieces.queens |= BIT << sq;
  }

  void addKing(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= BIT << sq;
    pieces.king = sq;
  }

  void setTurn(Side side)
  {
    if (side == WHITE)
    {
      state &= WHITE;
    }
    else
    {
      state |= BLACK;
    }
  }

  void addCastlingRight(Val cr)
  {
    state |= BIT << cr;
  }

private:
  BitBoards& getPieces(Side side)
  {
    assert(side == WHITE || side == BLACK);
    return side == WHITE ? whitePieces : blackPieces;
  }

  // For debugging purposes.
  void printAllBoards();

  BitBoards whitePieces;
  BitBoards blackPieces;

  // Bit 0: turn, Bit 1-4: Castling rights, 
  GameState state;
};