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
    pieces.all |= pieces.all << sq;
    pieces.pawns |= pieces.pawns << sq;
  }

  void addRook(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= pieces.all << sq;
    pieces.rooks |= pieces.rooks << sq;
  }

  void addKnight(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= pieces.all << sq;
    pieces.knights |= pieces.knights << sq;
  }

  void addBishop(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= pieces.all << sq;
    pieces.bishops |= pieces.bishops << sq;
  }

  void addQueen(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= pieces.all << sq;
    pieces.queens |= pieces.queens << sq;
  }

  void addKing(Square sq, Side side)
  {
    BitBoards& pieces = getPieces(side);
    pieces.all |= pieces.all << sq;
    pieces.king = sq;
  }


private:
  BitBoards& getPieces(Side side)
  {
    assert(side == WHITE || side == BLACK);
    return side == WHITE ? whitePieces : blackPieces;
  }

  BitBoards whitePieces;
  BitBoards blackPieces;
};