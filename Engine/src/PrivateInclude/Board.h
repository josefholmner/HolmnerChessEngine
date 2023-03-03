#pragma once

#include "PrivateInclude/PiecesSquaresAndMoves.h"

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

  bool operator==(const BitBoards& other) const
  {
    return all == other.all && pawns == other.pawns && rooks == other.rooks && knights == other.knights
      && bishops == other.bishops && queens == other.queens && king == other.king;
  }
};

class Board
{
public:
  bool init(const std::string& FEN);

  bool isValid() const;

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

  void setSideToPlay(Side side)
  {
    sideToPlay = side;
  }

  void addCastlingRight(Val cr)
  {
    castlingRights |= cr;
  }

  void removeCastlingRight(Val cr)
  {
    castlingRights &= ~cr;
  }

  void setHalfMoves(SmallVal inHalfMoves)
  {
    halfMoves = inHalfMoves;
  }

  void incrHalfMoves()
  {
    halfMoves++;
  }

  void decrHalfMoves()
  {
    halfMoves--;
  }

  void makeMove(Move move);
  void unMakeMove(Move move);

  bool operator==(const Board& other) const;

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

  Side sideToPlay{ WHITE };
  Val castlingRights{ CAN_CASTLE_WKWQBKBQ };
  Square enPassantTarget{ NO_SQUARE };
  SmallVal halfMoves{ 0 };

  // @todo cache allign this class once multithreading is added.
};