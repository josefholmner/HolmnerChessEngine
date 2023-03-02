#include "PrivateInclude/Board.h"

#include "Common/CommonUtilities.h"
#include "PrivateInclude/EngineUtilities.h"

#include <iostream>
#include <vector>

namespace Board_helpers
{
  bool setupPieces(const std::string& FENPieces, Board& board)
  {
    const std::vector<std::string> splitFEN = CommonUtilities::splitString(FENPieces, '/');
    if (splitFEN.size() != NUM_RANKS)
    {
      EngineUtilities::logE("setupPieces failed: wrong number of ranks in FEN.");
      return false;
    }

    for (Rank r = 0; r < NUM_RANKS; r++)
    {
      for (File f = 0; f < NUM_FILES; f++)
      {
        if (splitFEN[r].length() <= f)
        {
          EngineUtilities::logE("setupPieces failed: wrong number of files in FEN.");
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
            EngineUtilities::logE("setupPieces failed: invalid number of empty squares in FEN.");
            return false;
          }

          f += empties - 1;
          break;
        }
      }
    }

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
    EngineUtilities::logE("initFromFEN failed: Invalid FEN.");
    return false;
  }

  if (!setupPieces(splitFEN[0], *this))
  {
    return false;
  }

  // todo: read the rest of the FEN here.
  return false;
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
