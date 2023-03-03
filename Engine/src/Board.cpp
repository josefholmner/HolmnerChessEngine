#include "PrivateInclude/Board.h"

#include "Common/CommonUtilities.h"
#include "PrivateInclude/EngineUtilities.h"

#include <iostream>
#include <vector>
#include <unordered_map>

namespace Board_helpers
{
  bool setupPieces(const std::string& FENPieces, Board& board)
  {
    const std::vector<std::string> splitFEN = CommonUtilities::splitString(FENPieces, '/');
    if (splitFEN.size() != NUM_RANKS)
    {
      EngineUtilities::logE("setupPieces failed: wrong number of ranks in FEN: " + FENPieces);
      return false;
    }

    for (Rank r = 0; r < NUM_RANKS; r++)
    {
      for (File f = 0; f < NUM_FILES; f++)
      {
        if (splitFEN[r].length() <= f)
        {
          EngineUtilities::logE("setupPieces failed: wrong number of files in FEN: " + FENPieces);
          return false;
        }

        const char ch = splitFEN[(size_t)NUM_RANKS - (size_t)r - 1][f];
        const Square sq = r * NUM_FILES + f;
        switch (ch)
        {
        case 'p':
          board.addPawn(sq, BLACK);
          break;
        case 'r':
          board.addRook(sq, BLACK);
          break;
        case 'n':
          board.addKnight(sq, BLACK);
          break;
        case 'b':
          board.addBishop(sq, BLACK);
          break;
        case 'k':
          board.addKing(sq, BLACK);
          break;
        case 'q':
          board.addQueen(sq, BLACK);
          break;
        case 'P':
          board.addPawn(sq, WHITE);
          break;
        case 'R':
          board.addRook(sq, WHITE);
          break;
        case 'N':
          board.addKnight(sq, WHITE);
          break;
        case 'B':
          board.addBishop(sq, WHITE);
          break;
        case 'K':
          board.addKing(sq, WHITE);
          break;
        case 'Q':
          board.addQueen(sq, WHITE);
          break;
        default:
          Square empties = static_cast<Square>(ch - '0');
          if (empties > NUM_FILES - f || empties < 1)
          {
            EngineUtilities::logE("setupPieces failed: invalid number of empty squares in FEN: " + FENPieces);
            return false;
          }

          f += empties - 1;
          break;
        }
      }
    }

    return true;
  }

  bool setSide(const std::string& FENSide, Board& board)
  {
    if (FENSide.size() != 1)
    {
      EngineUtilities::logE("setSideToPlay failed: invalid FEN string: " + FENSide);
      return false;
    }

    if (FENSide[0] == 'w')
      board.setSideToPlay(WHITE);
    else if (FENSide[0] == 'b')
      board.setSideToPlay(BLACK);
    else
    {
      EngineUtilities::logE("setSideToPlay failed: invalid FEN string: " + FENSide);
      return false;
    }

    return true;
  }

  bool setCastlingRights(const std::string& FENCastling, Board& board)
  {
    if (FENCastling.size() == 0)
    {
      EngineUtilities::logE("setCastlingRights failed: invalid castling right string: " + FENCastling);
      return false;
    }

    board.removeCastlingRight(CAN_CASTLE_WKWQBKBQ);

    const std::unordered_map<char, Val> castlingMap{
      {'K', CAN_CASTLE_WK}, {'Q', CAN_CASTLE_WQ}, {'k', CAN_CASTLE_BK}, {'q', CAN_CASTLE_WQ} };
    for (char c : FENCastling)
    {
      auto res = castlingMap.find(c);
      if (res != castlingMap.end())
      {
        board.addCastlingRight(res->second);
      }
      else
      {
        EngineUtilities::logE("setCastlingRights failed: invalid castling right string" + FENCastling);
        return false;
      }
    }

    return true;
  }

  bool setEnPassantTargetSquare(const std::string& FENEnPassant, Board& board)
  {
    if (FENEnPassant == "-")
    {
      return NO_SQUARE;
    }

    if (FENEnPassant.size() != 2)
    {
      EngineUtilities::logE("Invalid square in FEN: " + FENEnPassant);
      return false;
    }

    Rank rank = static_cast<Rank>(FENEnPassant[1] - '1');
    File file = static_cast<File>(FENEnPassant[0] - 'a');

    if (rank < 0 || rank > NUM_RANKS || file < 0 || file > NUM_FILES)
    {
      EngineUtilities::logE("Invalid square in FEN: " + FENEnPassant);
      return false;
    }

    return file + NUM_FILES * rank;
  }

  bool setHalfMoveCount(const std::string& FENHalfMoves, Board& board)
  {
    if (FENHalfMoves.size() == 0)
    {
      EngineUtilities::logE("Invalid halfmove in FEN: " + FENHalfMoves);
      return false;
    }

    board.setHalfMoves(std::stoi(FENHalfMoves));
    return true;
  }
}

bool Board::init(const std::string& FEN)
{
  using namespace Board_helpers;
  const std::vector<std::string> splitFEN = CommonUtilities::splitString(FEN, ' ');

  // All FENs should be exactly 6 segments long.
  if (splitFEN.size() != 6)
  {
    EngineUtilities::logE("initFromFEN failed: Invalid FEN " + FEN);
    return false;
  }

  if (!setupPieces(splitFEN[0], *this))
  {
    return false;
  }

  if (!setSide(splitFEN[1], *this))
  {
    return false;
  }

  if (!setCastlingRights(splitFEN[2], *this))
  {
    return false;
  }

  if (!setEnPassantTargetSquare(splitFEN[3], *this))
  {
    return false;
  }

  if (!setHalfMoveCount(splitFEN[4], *this))
  {
    return false;
  }
  
  return true;
}

bool Board::isValid() const
{
  // @todo impl.
  return true;
}

void Board::makeMove(Move move)
{
  // todo: impl.
}

void Board::unMakeMove(Move move)
{
  // todo: impl.
}

bool Board::operator==(const Board& other) const
{
  return whitePieces == other.whitePieces && blackPieces == other.blackPieces &&
    sideToPlay == other.sideToPlay && castlingRights == other.castlingRights &&
    enPassantTarget == other.enPassantTarget && halfMoves == other.halfMoves;
}

void Board::printAllBoards()
{
  EngineUtilities::printBitBoard(whitePieces.all, "All White");
  EngineUtilities::printBitBoard(whitePieces.pawns, "White Pawns");
  EngineUtilities::printBitBoard(whitePieces.rooks, "White Rooks");
  EngineUtilities::printBitBoard(whitePieces.knights, "White Knights");
  EngineUtilities::printBitBoard(whitePieces.bishops, "White Bishops");
  EngineUtilities::printBitBoard(whitePieces.queens, "White Queens");
  EngineUtilities::printSquareOnBoard(whitePieces.king, "White King");

  EngineUtilities::printBitBoard(blackPieces.all, "All Black");
  EngineUtilities::printBitBoard(blackPieces.pawns, "Black Pawns");
  EngineUtilities::printBitBoard(blackPieces.rooks, "Blacks Rooks");
  EngineUtilities::printBitBoard(blackPieces.knights, "Blacks Knights");
  EngineUtilities::printBitBoard(blackPieces.bishops, "Blacks Bishops");
  EngineUtilities::printBitBoard(blackPieces.queens, "Blacks Queens");
  EngineUtilities::printSquareOnBoard(blackPieces.king, "Black King");
}
