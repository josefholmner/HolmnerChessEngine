#include "PrivateInclude/BoardEvaluator.h"

#include "PrivateInclude/EngineUtilities.h"

#include <cassert>
#include <vector>

/*
* Note on terminology: 
* 'vals' are always positive, thus must be negated for black.
* 'score' takes color into account, so is already negative for black.
*/

namespace scoringConstants
{
	static constexpr Score pawnProtectingKing1RankAwayVal = 40;
	static constexpr Score pawnProtectingKing2RanksAwayVal = 20;

	// The minimum number of opponent major pieces that has to be on the board for the king to
	// be rewarded to be "hidden" to the side of the board behind pawns.
	static constexpr Score minNumOpponentMajorPieceRewardKingSafety = 2;

	static constexpr Score doublePawnsPunishmentVal = 25;
	static constexpr Score pawnIslandPunishmentVal = 25;

	static constexpr Score rookOpenFileVal = 40;
	static constexpr Score trappedRookPenaltyVal = 80;

	static constexpr Score minorPieceProtectedByPawnVal = 10;

	static constexpr Score bishopCoverValPerSquare = 3;

	static constexpr std::array<Score, squares::num>bPawnStaticVals =
	{
		180, 180, 180, 200, 200, 180, 180, 180,
		180, 180, 180, 200, 200, 180, 180, 180,
		100, 120, 140, 160, 160, 140, 120, 100,
		100, 110, 120, 140, 140, 120, 110, 100,
		100, 110, 120, 140, 140, 120, 110, 100,
		100, 110, 120, 120, 120, 120, 110, 100,
		100, 110, 110, 110, 110, 110, 110, 100,
		100, 100, 100, 100, 100, 100, 100, 100
	};

	static constexpr std::array<Score, squares::num> wPawnStaticVals =
	{
		100, 100, 100, 100, 100, 100, 100, 100,
		100, 110, 110, 110, 110, 110, 110, 100,
		100, 110, 120, 120, 120, 120, 110, 100,
		100, 110, 120, 140, 140, 120, 110, 100,
		100, 110, 120, 140, 140, 120, 110, 100,
		100, 120, 140, 160, 160, 140, 120, 100,
		180, 180, 180, 200, 200, 180, 180, 180,
		180, 180, 180, 200, 200, 180, 180, 180
	};

	static constexpr std::array<Score, squares::num> bKnightStaticVals =
	{
		300, 300, 300, 300, 300, 300, 300, 300,
		300, 310, 310, 310, 310, 310, 310, 300,
		300, 310, 320, 320, 320, 320, 310, 300,
		300, 310, 320, 340, 340, 320, 310, 300,
		300, 310, 320, 340, 340, 320, 310, 300,
		300, 310, 320, 320, 320, 320, 310, 300,
		300, 310, 310, 310, 310, 310, 310, 300,
		300, 270, 300, 300, 300, 300, 270, 300
	};

	static constexpr std::array<Score, squares::num> wKnightStaticVals =
	{
		300, 270, 300, 300, 300, 300, 270, 300,
		300, 310, 310, 310, 310, 310, 310, 300,
		300, 310, 320, 320, 320, 320, 310, 300,
		300, 310, 320, 340, 340, 320, 310, 300,
		300, 310, 320, 340, 340, 320, 310, 300,
		300, 310, 320, 320, 320, 320, 310, 300,
		300, 310, 310, 310, 310, 310, 310, 300,
		300, 300, 300, 300, 300, 300, 300, 300
	};

	static constexpr std::array<Score, squares::num> bBishopStaticVals =
	{
		305, 305, 305, 305, 305, 305, 305, 305,
		305, 315, 315, 315, 315, 315, 315, 305,
		305, 325, 325, 325, 325, 325, 325, 305,
		305, 325, 335, 345, 345, 335, 325, 305,
		305, 325, 335, 345, 345, 335, 325, 305,
		305, 325, 325, 325, 325, 325, 325, 305,
		305, 315, 315, 315, 315, 315, 315, 305,
		305, 305, 275, 305, 305, 275, 305, 305
	};

	static constexpr std::array<Score, squares::num> wBishopStaticVals =
	{
		305, 305, 275, 305, 300, 275, 305, 305,
		305, 315, 315, 315, 315, 315, 315, 305,
		305, 325, 325, 325, 325, 325, 325, 305,
		305, 325, 335, 345, 345, 335, 325, 305,
		305, 325, 335, 345, 345, 335, 325, 305,
		305, 325, 325, 325, 325, 325, 325, 305,
		305, 315, 315, 315, 315, 315, 315, 305,
		305, 305, 305, 305, 305, 305, 305, 305
	};

	static constexpr std::array<Score, squares::num> rookStaticVals =
	{
		500, 507, 515, 522, 522, 515, 507, 500,
		500, 507, 515, 522, 522, 515, 507, 500,
		500, 507, 515, 522, 522, 515, 507, 500,
		500, 507, 515, 522, 522, 515, 507, 500,
		500, 507, 515, 522, 522, 515, 507, 500,
		500, 507, 515, 522, 522, 515, 507, 500,
		500, 507, 515, 522, 522, 515, 507, 500,
		500, 507, 515, 522, 522, 515, 507, 500
	};

	static constexpr std::array<Score, squares::num> bQueenStaticVals =
	{
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 875, 900, 900, 900, 900
	};

	static constexpr std::array<Score, squares::num> wQueenStaticVals =
	{
		900, 900, 900, 875, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900,
		900, 900, 900, 900, 900, 900, 900, 900
	};

	static constexpr std::array<Score, squares::num> bKingEndgameStaticVals =
	{
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10040, 10040, 10040, 10040, 10000, 10000,
		10000, 10000, 10040, 10050, 10050, 10040, 10000, 10000,
		10000, 10000, 10040, 10050, 10050, 10040, 10000, 10000,
		10000, 10000, 10040, 10040, 10040, 10040, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000
	};

	static constexpr std::array<Score, squares::num> wKingEndgameStaticVals =
	{
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10040, 10040, 10040, 10040, 10000, 10000,
		10000, 10000, 10040, 10050, 10050, 10040, 10000, 10000,
		10000, 10000, 10040, 10050, 10050, 10040, 10000, 10000,
		10000, 10000, 10040, 10040, 10040, 10040, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000
	};

	static constexpr std::array<Score, squares::num> bKingEarlyGameStaticVals =
	{
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10050, 10050, 10040, 10000, 10000, 10000, 10050, 10050
	};

	static constexpr std::array<Score, squares::num> wKingEarlyGameStaticVals =
	{
		10050, 10050, 10040, 10000, 10000, 10000, 10050, 10050,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
		10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000
	};
}

namespace
{
	bool isPawn(Piece piece)
	{
		switch (piece)
		{
			case pieces::wP:
			case pieces::bP:
				return true;
			default:
				return false;
		}
	}

	int8_t getNumNonPawnSquaresInSweep(const BoardState& board, const std::vector<Square>& direction,
		Square sq)
	{
		int8_t num = 0;
		for (const Square s : direction)
		{
			if (!isPawn(board.getPiece(s)))
			{
				num += 1;
			}
			else
			{
				return num;
			}
		}

		return num;
	}

	Score getWhiteBishopSquareCoverScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		const Score numSqs =
			getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsA8()[sq], sq) +
			getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsH8()[sq], sq) +
			getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsA1()[sq], sq) +
			getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsH1()[sq], sq);
		return numSqs * scoringConstants::bishopCoverValPerSquare;
	}

	Score getBlackBishopSquareCoverScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		const Score numSqs =
			-getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsA8()[sq], sq) -
			getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsH8()[sq], sq) -
			getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsA1()[sq], sq) -
			getNumNonPawnSquaresInSweep(board, lookup.getDiagTowardsH1()[sq], sq);
		return numSqs * scoringConstants::bishopCoverValPerSquare;
	}

	Score getWhiteMinorPiecePawnProtectedScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		using namespace scoringConstants;
		return 0;
		// Using black pawn capture pattern here which is not a bug.
		for (Square sq : lookup.getBlackPawnCaptureSquares()[sq])
		{
			if (board.getPiece(sq) == pieces::wP)
			{
				return minorPieceProtectedByPawnVal;
			}
		}

		return 0;
	}

	Score getBlackMinorPiecePawnProtectedScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		using namespace scoringConstants;
		return 0;
		// Using white pawn capture pattern here which is not a bug.
		for (Square sq : lookup.getWhitePawnCaptureSquares()[sq])
		{
			if (board.getPiece(sq) == pieces::bP)
			{
				return -minorPieceProtectedByPawnVal;
			}
		}

		return 0;
	}

	BoardEvaluator::PreMoveInfo createFromMovingSidePawn(const BoardState& board, Piece movingSidePawn)
	{
		BoardEvaluator::PreMoveInfo info;

		for (Square sq = squares::a2; sq <= squares::h7; sq++)
		{
			assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
			assert(EngineUtilities::isNonNoneSquare(sq));

			if (board.getPiece(sq) == movingSidePawn)
			{
				info.movingSidePawnsFileOccupation[files::toFile(sq)] = true;
			}
		}

		return info;
	}

	Score getPawnIslandsVal(const std::array<bool, files::num>& filesOccupied, Score val)
	{
		Score score = 0;
		if (filesOccupied[files::fileA] && !filesOccupied[files::fileB])
		{
			score += val;
		}

		if (!filesOccupied[files::fileG] && filesOccupied[files::fileH])
		{
			score += val;
		}

		for (File f = files::fileC; f < files::num; f++)
		{
			if (!filesOccupied[(size_t)f - 2] && filesOccupied[(size_t)f - 1] && !filesOccupied[f])
			{
				score += val;
			}
		}

		return score;
	}

	bool isNonPawnNonKing(Piece piece)
	{
		switch (piece)
		{
			case pieces::wP:
			case pieces::bP:
			case pieces::wK:
			case pieces::bK:
				return false;
			default:
				return true;
		}
	}

	Score getWhiteKingScore(const BoardState& board, int8_t numBlackMajorPieces)
	{
		using namespace scoringConstants;

		if (numBlackMajorPieces < minNumOpponentMajorPieceRewardKingSafety)
		{
			// The kBaseVal is baked into the array.
			return wKingEndgameStaticVals[board.getWhiteKingSquare()];
		}

		// Non-endgame: reward king protection.
		Score score = 0;
		switch (board.getWhiteKingSquare())
		{
		case squares::a1:
		case squares::b1:
		{
			if (board.getPiece(squares::a2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::a3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::b2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::b3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::c2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::c3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

			// Trapped rook penalty.
			if (board.getPiece(squares::a1) == pieces::wR)
			{
				score -= trappedRookPenaltyVal;
			}
		}
		break;
		case squares::g1:
		case squares::h1:
		{
			if (board.getPiece(squares::f2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::f3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::g2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::g3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::h2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::h3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

			// Trapped rook penalty.
			if (board.getPiece(squares::h1) == pieces::wR)
			{
				score -= trappedRookPenaltyVal;
			}
		}
		}

		return score + wKingEarlyGameStaticVals[board.getWhiteKingSquare()];
	}

	Score getBlackKingScore(const BoardState& board, int8_t numWhiteMajorPieces)
	{
		using namespace scoringConstants;

		if (numWhiteMajorPieces < minNumOpponentMajorPieceRewardKingSafety)
		{
			// The kBaseVal is baked into the array.
			return -bKingEndgameStaticVals[board.getBlackKingSquare()];
		}

		// Non-endgame: reward king protection.
		Score score = 0;
		switch (board.getBlackKingSquare())
		{
		case squares::a8:
		case squares::b8:
		{
			if (board.getPiece(squares::a7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::a6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::b7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::b6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::c7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::c6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

			// Trapped rook penalty.
			if (board.getPiece(squares::a8) == pieces::bR)
			{
				score += trappedRookPenaltyVal;
			}
		}
		break;
		case squares::g8:
		case squares::h8:
		{
			if (board.getPiece(squares::f7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::f6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::g7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::g6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

			if (board.getPiece(squares::h7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
			else if (board.getPiece(squares::h6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

			// Trapped rook penalty.
			if (board.getPiece(squares::h8) == pieces::bR)
			{
				score += trappedRookPenaltyVal;
			}
		}
		}

		return score - bKingEarlyGameStaticVals[board.getBlackKingSquare()];
	}

	Score getWhiteKnightMoveDeltaScore(const BoardState& board, const Move& move, const FastSqLookup& lookup)
	{
		using namespace scoringConstants;
		Score score = wKnightStaticVals[move.toSquare] - wKnightStaticVals[move.fromSquare];
		score += getWhiteMinorPiecePawnProtectedScore(board, lookup, move.toSquare);
		score -= getWhiteMinorPiecePawnProtectedScore(board, lookup, move.fromSquare);
		return score;
	}

	Score getBlackKnightMoveDeltaScore(const BoardState& board, const Move& move, const FastSqLookup& lookup)
	{
		using namespace scoringConstants;
		Score score = -bKnightStaticVals[move.toSquare] + bKnightStaticVals[move.fromSquare];
		score += getBlackMinorPiecePawnProtectedScore(board, lookup, move.toSquare);
		score -= getBlackMinorPiecePawnProtectedScore(board, lookup, move.fromSquare);
		return score;
	}

	Score getWhiteBishopMoveDeltaScore(const BoardState& board, const Move& move,
		const FastSqLookup& lookup)
	{
		using namespace scoringConstants;
		Score score = wBishopStaticVals[move.toSquare] - wBishopStaticVals[move.fromSquare];
		score += getWhiteBishopSquareCoverScore(board, lookup, move.toSquare);
		score -= getWhiteBishopSquareCoverScore(board, lookup, move.fromSquare);
		score += getWhiteMinorPiecePawnProtectedScore(board, lookup, move.toSquare);
		score -= getWhiteMinorPiecePawnProtectedScore(board, lookup, move.fromSquare);
		return score;
	}

	Score getBlackBishopMoveDeltaScore(const BoardState& board, const Move& move,
		const FastSqLookup& lookup)
	{
		using namespace scoringConstants;
		Score score = -bBishopStaticVals[move.toSquare] + bBishopStaticVals[move.fromSquare];
		score += getBlackBishopSquareCoverScore(board, lookup, move.toSquare);
		score -= getBlackBishopSquareCoverScore(board, lookup, move.fromSquare);
		score += getBlackMinorPiecePawnProtectedScore(board, lookup, move.toSquare);
		score -= getBlackMinorPiecePawnProtectedScore(board, lookup, move.fromSquare);
		return score;
	}

	Score getWhiteRookMoveDeltaScore(const BoardState& board, const Move& move,
		const BoardEvaluator::PreMoveInfo& preMoveInfo, const FastSqLookup& lookup)
	{
		using namespace scoringConstants;
		Score score = rookStaticVals[move.toSquare] - rookStaticVals[move.fromSquare];

		// Handle change in open file score.
		const File newFile = files::toFile(move.toSquare);
		const File oldFile = files::toFile(move.fromSquare);
		if (!preMoveInfo.movingSidePawnsFileOccupation[newFile])
		{
			score += scoringConstants::rookOpenFileVal;
		}

		if (!preMoveInfo.movingSidePawnsFileOccupation[oldFile])
		{
			score -= scoringConstants::rookOpenFileVal;
		}

		return score;
	}

	Score getBlackRookMoveDeltaScore(const BoardState& board, const Move& move,
		const BoardEvaluator::PreMoveInfo& preMoveInfo, const FastSqLookup& lookup)
	{
		using namespace scoringConstants;
		Score score = -rookStaticVals[move.toSquare] + rookStaticVals[move.fromSquare];

		// Handle change in open file score.
		const File newFile = files::toFile(move.toSquare);
		const File oldFile = files::toFile(move.fromSquare);
		if (!preMoveInfo.movingSidePawnsFileOccupation[newFile])
		{
			score -= scoringConstants::rookOpenFileVal;
		}

		if (!preMoveInfo.movingSidePawnsFileOccupation[oldFile])
		{
			score += scoringConstants::rookOpenFileVal;
		}

		return score;
	}

	Score getPieceMoveDeltaScore(const BoardState& board, const Move& move,
		const BoardEvaluator::PreMoveInfo& preMoveInfo, const FastSqLookup& lookup)
	{
		using namespace scoringConstants;
		assert(BoardEvaluator::canUseGetStaticEvaluationDelta(move));

		switch (move.movingPiece)
		{
		case pieces::wN:
			return getWhiteKnightMoveDeltaScore(board, move, lookup);
		case pieces::bN:
			return getBlackKnightMoveDeltaScore(board, move, lookup);
		case pieces::wB:
			return getWhiteBishopMoveDeltaScore(board, move, lookup);
		case pieces::bB:
			return getBlackBishopMoveDeltaScore(board, move, lookup);
		case pieces::wR:
			return getWhiteRookMoveDeltaScore(board, move, preMoveInfo, lookup);
		case pieces::bR:
			return getBlackRookMoveDeltaScore(board, move, preMoveInfo, lookup);
		case pieces::wQ:
			return wQueenStaticVals[move.toSquare] - wQueenStaticVals[move.fromSquare];
		case pieces::bQ:
			return -bQueenStaticVals[move.toSquare] + bQueenStaticVals[move.fromSquare];
		default:
			EngineUtilities::logE("Piece passed to getPieceMoveDeltaScore that cannot be used.");
			return 0;
		}
	}
}

Score BoardEvaluator::getStaticEvaluation(const BoardState& board, const FastSqLookup& lookup)
{
	using namespace scoringConstants;

	Score score = 0;
	int8_t numWhiteMajorPieces = 0;
	int8_t numBlackMajorPieces = 0;

	// Theoretical max number of rooks is 10.
	static std::array<Square, 10> wRooks;
	static std::array<Square, 10> bRooks;
	int8_t wRookCnt = 0;
	int8_t bRookCnt = 0;

	std::array<bool, files::num> filesOccupiedBybP = { false };
	std::array<bool, files::num> filesOccupiedBywP = { false };
	
	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
		assert(EngineUtilities::isNonNoneSquare(sq));

		switch (board.getPiece(sq))
		{
			case pieces::wP:
			{
				score += wPawnStaticVals[sq];
				const File f = files::toFile(sq);
				if (filesOccupiedBywP[f])
				{
					score -= doublePawnsPunishmentVal;
				}
				else
				{
					filesOccupiedBywP[f] = true;
				}
			}
				break;
			case pieces::wN:
				numWhiteMajorPieces++;
				score += wKnightStaticVals[sq];
				score += getWhiteMinorPiecePawnProtectedScore(board, lookup, sq);
				break;
			case pieces::wB:
				numWhiteMajorPieces++;
				score += wBishopStaticVals[sq];
				score += getWhiteBishopSquareCoverScore(board, lookup, sq);
				score += getWhiteMinorPiecePawnProtectedScore(board, lookup, sq);
				break;
			case pieces::wR:
				numWhiteMajorPieces++;
				wRooks[wRookCnt++] = sq;
				score += rookStaticVals[sq];
				break;
			case pieces::wQ:
				numWhiteMajorPieces++;
				score += wQueenStaticVals[sq];
				break;
			case pieces::wK:
				// King scores are handled elsewhere.
				break;
			case pieces::bP:
			{
				score -= bPawnStaticVals[sq];
				const File f = files::toFile(sq);
				if (filesOccupiedBybP[f])
				{
					score += scoringConstants::doublePawnsPunishmentVal;
				}
				else
				{
					filesOccupiedBybP[f] = true;
				}
			}
				break;
			case pieces::bN:
				numBlackMajorPieces++;
				score -= bKnightStaticVals[sq];
				score += getBlackMinorPiecePawnProtectedScore(board, lookup, sq);
				break;
			case pieces::bB:
				numBlackMajorPieces++;
				score -= bBishopStaticVals[sq];
				score += getBlackBishopSquareCoverScore(board, lookup, sq);
				score += getBlackMinorPiecePawnProtectedScore(board, lookup, sq);
				break;
			case pieces::bR:
				bRooks[bRookCnt++] = sq;
				numBlackMajorPieces++;
				score -= rookStaticVals[sq];
				break;
			case pieces::bQ:
				numBlackMajorPieces++;
				score -= bQueenStaticVals[sq];
				break;
			case pieces::bK:
				// King scores are handled elsewhere.
				break;
		}
	}

	// Handle king scores.
	score += getWhiteKingScore(board, numBlackMajorPieces);
	score += getBlackKingScore(board, numWhiteMajorPieces);

	// Give score for rook on open file.
	for (int8_t i = 0; i < wRookCnt; i++)
	{
		if (!filesOccupiedBywP[files::toFile(wRooks[i])])
		{
			score += scoringConstants::rookOpenFileVal;
		}
	}

	for (int8_t i = 0; i < bRookCnt; i++)
	{
		if (!filesOccupiedBybP[files::toFile(bRooks[i])])
		{
			score -= scoringConstants::rookOpenFileVal;
		}
	}

	// Punish pawn islands.
	score -= getPawnIslandsVal(filesOccupiedBywP, scoringConstants::pawnIslandPunishmentVal);
	score += getPawnIslandsVal(filesOccupiedBybP, scoringConstants::pawnIslandPunishmentVal);

	// Negate the score for black, so that a high score is always "good" for both sides.
	// This makes negaMax style recursive functions possible.
	return board.getTurn() == pieces::Color::WHITE ? score : -score;
}

Score BoardEvaluator::getStaticEvaluationDelta(const BoardState& board, const Move& move,
	const PreMoveInfo& preMoveInfo, const FastSqLookup& lookup)
{
assert(canUseGetStaticEvaluationDelta(move));

	const Score delta = getPieceMoveDeltaScore(board, move, preMoveInfo, lookup);
	return board.getTurn() == pieces::Color::WHITE ? delta : -delta;
}

bool BoardEvaluator::canUseGetStaticEvaluationDelta(const Move& move)
{
	assert(EngineUtilities::isNonNonePiece(move.movingPiece));

	if (move.capturedPiece != pieces::none)
	{
		return false;
	}

	return isNonPawnNonKing(move.movingPiece);
}

BoardEvaluator::PreMoveInfo BoardEvaluator::createPreMoveInfo(const BoardState& board)
{
	const Piece movingSidePawn = board.getTurn() == pieces::Color::WHITE ? pieces::wP : pieces::bP;
	return createFromMovingSidePawn(board, movingSidePawn);
}

Score BoardEvaluator::getPawnBaseValue()
{
	return 100;
}
