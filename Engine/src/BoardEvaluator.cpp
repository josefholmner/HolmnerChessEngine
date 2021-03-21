#include "PrivateInclude/BoardEvaluator.h"

#include "PrivateInclude/EngineUtilities.h"
#include "PrivateInclude/BoardState.h"
#include "PrivateInclude/FastSqLookup.h"

#include <cassert>
#include <vector>

namespace scoringConstants
{
	static constexpr int32_t rankScoreMin = 0;
	static constexpr int32_t fileScoreMin = 0;

	// How much more a square on a rank or file 1 rank or file closer to the board center is worth.
	static constexpr int32_t rankScoreJump = 2;
	static constexpr int32_t fileScoreJump = 2;

	static constexpr int32_t pBaseVal = 10;
	static constexpr int32_t nBaseVal = pBaseVal * 3;
	static constexpr int32_t bBaseVal = pBaseVal * 3;
	static constexpr int32_t rBaseVal = pBaseVal * 5;
	static constexpr int32_t qBaseVal = pBaseVal * 9;

	static constexpr int32_t kingToTheSideVal = 9;
	static constexpr int32_t pawnProtectingKing1RankAwayVal = 9;
	static constexpr int32_t pawnProtectingKing2RanksAwayVal = 4;

	//static constexpr int32_t rookCoverValPerSquare = 1;
	//static constexpr int32_t bishopCoverValPerSquare = 1;
}

namespace
{
	int32_t getRankScoreRewardCenter(Rank rank)
	{
		using namespace scoringConstants;
		switch (rank)
		{
		case 0:
		case 7:
			return rankScoreMin;
		case 1:
		case 6:
			return rankScoreMin + 1 * rankScoreJump;
		case 2:
		case 5:
			return rankScoreMin + 2 * rankScoreJump;
		case 3:
		case 4:
			return rankScoreMin + 3 * rankScoreJump;
		default:
			EngineUtilities::logE("BoardEvaluator: invalid rank passed to getRankScore().");
			return 0;
		}
	}

	int32_t getRankScoreRewardTowardsRankMax(Rank rank)
	{
		using namespace scoringConstants;
		return rankScoreMin + rank * rankScoreJump;
	}

	int32_t getRankScoreRewardTowardsRankMin(Rank rank)
	{
		using namespace scoringConstants;
		return rankScoreMin + (7 - rank) * rankScoreJump;
	}

	int32_t getFileScore(File file)
	{
		using namespace scoringConstants;
		switch (file)
		{
		case 0:
		case 7:
			return fileScoreMin;
		case 1:
		case 6:
			return fileScoreMin + 1 * fileScoreJump;
		case 2:
		case 5:
			return fileScoreMin + 2 * fileScoreJump;
		case 3:
		case 4:
			return fileScoreMin + 3 * fileScoreJump;
		default:
			EngineUtilities::logE("BoardEvaluator: invalid file passed to getFileScore().");
			return 0;
		}
	}

	int32_t getNumEmptySquaresInSweep(const BoardState& board, const std::vector<Square>& direction,
		Square sq)
	{
		int32_t num = 0;
		for (const Square s : direction)
		{
			if (board.getPiece(s) == pieces::none)
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

	int32_t getWhiteRookSquareCoverForwardsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return getNumEmptySquaresInSweep(board, lookup.getstraightTowardsRankMax()[sq], sq);
	}

	int32_t getBlackRookSquareCoverForwardsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return -getNumEmptySquaresInSweep(board, lookup.getstraightTowardsRankMin()[sq], sq);
	}

	int32_t getWhiteRookSquareCoverAllDirsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return getNumEmptySquaresInSweep(board, lookup.getstraightTowardsRankMax()[sq], sq) + 
			getNumEmptySquaresInSweep(board, lookup.getstraightTowardsRankMin()[sq], sq) +
			getNumEmptySquaresInSweep(board, lookup.getstraightTowardsFileMax()[sq], sq) + 
			getNumEmptySquaresInSweep(board, lookup.getstraightTowardsFileMin()[sq], sq);
	}

	int32_t getBlackRookSquareCoverAllDirsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return -getNumEmptySquaresInSweep(board, lookup.getstraightTowardsRankMax()[sq], sq) -
			getNumEmptySquaresInSweep(board, lookup.getstraightTowardsRankMin()[sq], sq) -
			getNumEmptySquaresInSweep(board, lookup.getstraightTowardsFileMax()[sq], sq) -
			getNumEmptySquaresInSweep(board, lookup.getstraightTowardsFileMin()[sq], sq);
	}

	int32_t getWhiteBishopSquareCoverForwardsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return getNumEmptySquaresInSweep(board, lookup.getDiagTowardsA8()[sq], sq) + 
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsH8()[sq], sq);
	}

	int32_t getBlackBishopSquareCoverForwardsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return -getNumEmptySquaresInSweep(board, lookup.getDiagTowardsA1()[sq], sq) -
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsH1()[sq], sq);
	}

	int32_t getWhiteBishopSquareCoverAllDirsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return getNumEmptySquaresInSweep(board, lookup.getDiagTowardsA8()[sq], sq) +
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsH8()[sq], sq) +
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsA1()[sq], sq) +
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsH1()[sq], sq);
	}

	int32_t getBlackBishopSquareCoverAllDirsScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq)
	{
		return -getNumEmptySquaresInSweep(board, lookup.getDiagTowardsA8()[sq], sq) -
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsH8()[sq], sq) -
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsA1()[sq], sq) -
			getNumEmptySquaresInSweep(board, lookup.getDiagTowardsH1()[sq], sq);
	}

	int32_t getWhiteRookSquareCoverScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq, int32_t numBlackMajorPieces)
	{
		if (numBlackMajorPieces <= 1)
		{
			return getWhiteRookSquareCoverAllDirsScore(board, lookup, sq);
		}

		return getWhiteRookSquareCoverForwardsScore(board, lookup, sq);
	}

	int32_t getBlackRookSquareCoverScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq, int32_t numWhiteMajorPieces)
	{
		if (numWhiteMajorPieces <= 1)
		{
			return getBlackRookSquareCoverAllDirsScore(board, lookup, sq);
		}

		return getBlackRookSquareCoverForwardsScore(board, lookup, sq);
	}

	int32_t getWhiteBishopSquareCoverScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq, int32_t numBlackMajorPieces)
	{
		if (numBlackMajorPieces <= 1)
		{
			return getWhiteBishopSquareCoverAllDirsScore(board, lookup, sq);
		}

		return getWhiteBishopSquareCoverForwardsScore(board, lookup, sq);
	}

	int32_t getBlackBishopSquareCoverScore(const BoardState& board, const FastSqLookup& lookup,
		Square sq, int32_t numWhiteMajorPieces)
	{
		if (numWhiteMajorPieces <= 1)
		{
			return getBlackBishopSquareCoverAllDirsScore(board, lookup, sq);
		}

		return getBlackBishopSquareCoverForwardsScore(board, lookup, sq);
	}
}

BoardEvaluator::BoardEvaluator()
{
	init();
}

int32_t BoardEvaluator::getScore(const BoardState& board, const FastSqLookup& lookup) const
{
	int32_t score = 0;
	int32_t numWhiteMajorPieces = 0;
	int32_t numBlackMajorPieces = 0;

	std::vector<Square> wRooks;
	std::vector<Square> bRooks;
	std::vector<Square> wBishops;
	std::vector<Square> bBishops;
	wRooks.reserve(10); // Theoretical max number of rooks.
	bRooks.reserve(10); // Theoretical max number of rooks.
	wBishops.reserve(10); // Theoretical max number of bishops.
	bBishops.reserve(10); // Theoretical max number of bishops.
	
	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		assert(EngineUtilities::isValidPiece(board.getPiece(sq)));
		assert(EngineUtilities::isNonNoneSquare(sq));

		switch (board.getPiece(sq))
		{
			case pieces::wP:
				score += wPawnStaticScores[sq];
				break;
			case pieces::wN:
				numWhiteMajorPieces++;
				score += wKnightStaticScores[sq];
				break;
			case pieces::wB:
				wBishops.push_back(sq);
				numWhiteMajorPieces++;
				score += wBishopStaticScores[sq];
				break;
			case pieces::wR:
				numWhiteMajorPieces++;
				wRooks.push_back(sq);
				score += wRookStaticScores[sq];
				break;
			case pieces::wQ:
				numWhiteMajorPieces++;
				score += wQueenStaticScores[sq];
				break;
			case pieces::wK:
				// King scores are handled elsewhere.
				break;
			case pieces::bP:
				score += bPawnStaticScores[sq];
				break;
			case pieces::bN:
				numBlackMajorPieces++;
				score += bKnightStaticScores[sq];
				break;
			case pieces::bB:
				bBishops.push_back(sq);
				numBlackMajorPieces++;
				score += bBishopStaticScores[sq];
				break;
			case pieces::bR:
				bRooks.push_back(sq);
				numBlackMajorPieces++;
				score += bRookStaticScores[sq];
				break;
			case pieces::bQ:
				numBlackMajorPieces++;
				score += bQueenStaticScores[sq];
				break;
			case pieces::bK:
				// King scores are handled elsewhere.
				break;
		}
	}

	// Handle king scores.
	score += getWhiteKingScore(board, numBlackMajorPieces);
	score += getBlackKingScore(board, numWhiteMajorPieces);

	// Give score for rook and bishop square coverage.
	for (const Square s : wRooks) {
		score += getWhiteRookSquareCoverScore(board, lookup, s, numBlackMajorPieces);
	}
	for (const Square s : bRooks) {
		score += getBlackRookSquareCoverScore(board, lookup, s, numWhiteMajorPieces);
	}
	for (const Square s : wBishops) {
		score += getWhiteBishopSquareCoverScore(board, lookup, s, numBlackMajorPieces);
	}
	for (const Square s : bBishops) {
		score += getBlackBishopSquareCoverScore(board, lookup, s, numWhiteMajorPieces);
	}

	return score;
}

void BoardEvaluator::init()
{
	using namespace scoringConstants;

	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		const int32_t rankBonusRewardCenter = getRankScoreRewardCenter(ranks::toRank(sq));
		const int32_t rankBonusRewardRankMax = getRankScoreRewardTowardsRankMax(ranks::toRank(sq));
		const int32_t rankBonusRewardRankMin = getRankScoreRewardTowardsRankMin(ranks::toRank(sq));
		const int32_t fileBonus = getFileScore(files::toFile(sq));

		wKingEndgameStaticScores[sq] = rankBonusRewardCenter + fileBonus;
		bKingEndgameStaticScores[sq] = -rankBonusRewardCenter - fileBonus;
		wQueenStaticScores[sq] = qBaseVal;
		bQueenStaticScores[sq] = -qBaseVal;
		wRookStaticScores[sq] = rBaseVal;
		bRookStaticScores[sq] = -rBaseVal;
		wBishopStaticScores[sq] = bBaseVal;
		bBishopStaticScores[sq] = -bBaseVal;
		wKnightStaticScores[sq] = nBaseVal + rankBonusRewardCenter + fileBonus;
		bKnightStaticScores[sq] = -nBaseVal - rankBonusRewardCenter - fileBonus;
		wPawnStaticScores[sq] = pBaseVal + rankBonusRewardRankMax + fileBonus;
		bPawnStaticScores[sq] = -pBaseVal - rankBonusRewardRankMin - fileBonus;
	}
}

int32_t BoardEvaluator::getWhiteKingScore(const BoardState& board, int32_t numBlackMajorPieces) const
{
	using namespace scoringConstants;

	if (numBlackMajorPieces <= 1)
	{
		return wKingEndgameStaticScores[board.getWhiteKingSquare()];
	}

	// Non-endgame: reward king protection.
	int32_t score = 0;
	switch (board.getWhiteKingSquare())
	{
	case squares::a1:
	case squares::b1:
	{
		score += kingToTheSideVal;
		if (board.getPiece(squares::a2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::a3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }
			
		if (board.getPiece(squares::b2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::b3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }
			
		if (board.getPiece(squares::c2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::c3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }
	}
	break;
	case squares::g1:
	case squares::h1:
	{
		score += kingToTheSideVal;
		if (board.getPiece(squares::f2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::f3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

		if (board.getPiece(squares::g2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::g3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }

		if (board.getPiece(squares::h2) == pieces::wP) { score += pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::h3) == pieces::wP) { score += pawnProtectingKing2RanksAwayVal; }
	}
	}

	return score;
}

int32_t BoardEvaluator::getBlackKingScore(const BoardState& board, int32_t numWhiteMajorPieces) const
{
	using namespace scoringConstants;

	if (numWhiteMajorPieces <= 1)
	{
		return -wKingEndgameStaticScores[board.getBlackKingSquare()];
	}

	// Non-endgame: reward king protection.
	int32_t score = 0;
	switch (board.getBlackKingSquare())
	{
	case squares::a8:
	case squares::b8:
	{
		score -= kingToTheSideVal;
		if (board.getPiece(squares::a7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::a6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

		if (board.getPiece(squares::b7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::b6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

		if (board.getPiece(squares::c7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::c6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }
	}
	break;
	case squares::g8:
	case squares::h8:
	{
		score -= kingToTheSideVal;
		if (board.getPiece(squares::f7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::f6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

		if (board.getPiece(squares::g7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::g6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }

		if (board.getPiece(squares::h7) == pieces::bP) { score -= pawnProtectingKing1RankAwayVal; }
		else if (board.getPiece(squares::h6) == pieces::bP) { score -= pawnProtectingKing2RanksAwayVal; }
	}
	}

	return score;
}
