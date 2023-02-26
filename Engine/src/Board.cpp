#include "PrivateInclude/Board.h"

#include "Common/CommonUtilities.h"
#include "PrivateInclude/EngineUtilities.h"

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

        const char ch = splitFEN[r][f];
        const Square sq = r * NUM_FILES - 1 + f;
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
          board.addKing(sq, WHITE);
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

  return false;
}