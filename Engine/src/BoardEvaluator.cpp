#include "PrivateInclude/BoardEvaluator.h"

#include "PrivateInclude/EngineUtilities.h"

#include <cassert>
#include <vector>

namespace scoringConstants
{
	static constexpr Score rankScoreMin = 0;
	static constexpr Score fileScoreMin = 0;

	// How much more a square on a rank or file 1 rank or file closer to the board center is worth.
	static constexpr Score rankScoreJump = 6;
	static constexpr Score fileScoreJump = 6;

	static constexpr Score pBaseVal = 100;
	static constexpr Score nBaseVal = 320;
	static constexpr Score bBaseVal = 330;
	static constexpr Score rBaseVal = 500;
	static constexpr Score qBaseVal = 900;
	static constexpr Score kBaseVal = 10000;

	static constexpr Score kingToTheSideVal = 60;
	static constexpr Score pawnProtectingKing1RankAwayVal = 30;
	static constexpr Score pawnProtectingKing2RanksAwayVal = 20;

	// The minimum number of opponent major pieces that has to be on the board for the king to
	// be rewarded to be "hidden" to the side of the board behind pawns.
	static constexpr Score minNumOpponentMajorPieceRewardKingSafety = 2;


	static constexpr Score doublePawnsPunishmentVal = 25;
	static constexpr Score pawnIslandPunishmentVal = 25;

	static constexpr Score rookOpenFileVal = 30;

	static constexpr Score bishopCoverValPerSquare = 3;
}

namespace
{
	Score getRankScoreRewardCenter(Rank rank)
	{
		using namespace scoringConstants;
		switch (rank)
		{
			case ranks::rank1:
			case ranks::rank8:
				return rankScoreMin;
			case ranks::rank2:
			case ranks::rank7:
				return rankScoreMin + 1 * rankScoreJump;
			case ranks::rank3:
			case ranks::rank6:
				return rankScoreMin + 2 * rankScoreJump;
			case ranks::rank4:
			case ranks::rank5:
				return rankScoreMin + 3 * rankScoreJump;
			default:
				EngineUtilities::logE("BoardEvaluator: invalid rank passed to getRankScore().");
				return 0;
		}
	}

	Score getFileScore(File file)
	{
		using namespace scoringConstants;
		switch (file)
		{
			case files::fileA:
			case files::fileH:
				return fileScoreMin;
			case files::fileB:
			case files::fileG:
				return fileScoreMin + 1 * fileScoreJump;
			case files::fileC:
			case files::fileF:
				return fileScoreMin + 2 * fileScoreJump;
			case files::fileD:
			case files::fileE:
				return fileScoreMin + 3 * fileScoreJump;
			default:
				EngineUtilities::logE("BoardEvaluator: invalid file passed to getFileScore().");
				return 0;
		}
	}

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
}

BoardEvaluator::BoardEvaluator()
{
	init();
}

Score BoardEvaluator::getStaticEvaluation(const BoardState& board, const FastSqLookup& lookup) const
{
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
				score += wPawnStaticScores[sq];
				const File f = files::toFile(sq);
				if (filesOccupiedBywP[f])
				{
					score -= scoringConstants::doublePawnsPunishmentVal;
				}
				else
				{
					filesOccupiedBywP[f] = true;
				}
			}
				break;
			case pieces::wN:
				numWhiteMajorPieces++;
				score += wKnightStaticScores[sq];
				break;
			case pieces::wB:
				numWhiteMajorPieces++;
				score += wBishopStaticScores[sq];
				score += getWhiteBishopSquareCoverScore(board, lookup, sq);
				break;
			case pieces::wR:
				numWhiteMajorPieces++;
				wRooks[wRookCnt++] = sq;
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
			{
				score += bPawnStaticScores[sq];
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
				score += bKnightStaticScores[sq];
				break;
			case pieces::bB:
				numBlackMajorPieces++;
				score += bBishopStaticScores[sq];
				score += getBlackBishopSquareCoverScore(board, lookup, sq);
				break;
			case pieces::bR:
				bRooks[bRookCnt++] = sq;
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
	const PreMoveInfo& preMoveInfo, const FastSqLookup& lookup) const
{
	assert(canUseGetStaticEvaluationDelta(move));

	const Score delta = getPieceMoveDeltaScore(board, move, preMoveInfo, lookup);
	return board.getTurn() == pieces::Color::WHITE ? delta : -delta;
}

bool BoardEvaluator::canUseGetStaticEvaluationDelta(const Move& move) const
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
	return scoringConstants::pBaseVal;
}

void BoardEvaluator::init()
{
	using namespace scoringConstants;

	// Set pawn bonus scores (base value added later).
	bPawnStaticScores =
	{
		-0,  -0,  -0,  -0,  -0,  -0,  -0,  -0,
	    -70, -70, -70, -70, -70, -70, -70, -70,
		-0,  -20, -40, -60, -60, -40, -20, -0,
		-0,  -10, -20, -40, -40, -20, -10, -0,
		-0,  -10, -20, -40, -40, -20, -10, -0,
		-0,  -10, -20, -20, -20, -20, -10, -0,
		-0,  -10, -10, -10, -10, -10, -10, -0,
		-0,  -0,  -0,  -0,  -0,  -0,  -0,  -0
	};

	wPawnStaticScores =
	{
		0,  0,  0,  0,  0,  0,  0,  0,
		0,  10, 10, 10, 10, 10, 10, 0,
		0,  10, 20, 20, 20, 20, 10, 0,
		0,  10, 20, 40, 40, 20, 10, 0,
		0,  10, 20, 40, 40, 20, 10, 0,
		0,  20, 40, 60, 60, 40, 20, 0,
		70, 70, 70, 70, 70, 70, 70, 70,
		0,  0,  0,  0,  0,  0,  0,  0
	};

	for (Square sq = squares::a1; sq < squares::num; sq++)
	{
		const Score rankBonus = getRankScoreRewardCenter(ranks::toRank(sq));
		const Score fileBonus = getFileScore(files::toFile(sq));

		wKingEndgameStaticScores[sq] = kBaseVal + rankBonus + fileBonus;
		bKingEndgameStaticScores[sq] = -kBaseVal -rankBonus - fileBonus;
		wQueenStaticScores[sq] = qBaseVal;
		bQueenStaticScores[sq] = -qBaseVal;
		wRookStaticScores[sq] = rBaseVal + fileBonus;
		bRookStaticScores[sq] = -rBaseVal - fileBonus;
		wBishopStaticScores[sq] = bBaseVal + rankBonus + fileBonus;
		bBishopStaticScores[sq] = -bBaseVal - rankBonus - fileBonus;
		wKnightStaticScores[sq] = nBaseVal + rankBonus + fileBonus;
		bKnightStaticScores[sq] = -nBaseVal - rankBonus - fileBonus;
		wPawnStaticScores[sq] += pBaseVal;
		bPawnStaticScores[sq] -= pBaseVal;
	}
}

Score BoardEvaluator::getWhiteKingScore(const BoardState& board, int8_t numBlackMajorPieces) const
{
	using namespace scoringConstants;

	if (numBlackMajorPieces < minNumOpponentMajorPieceRewardKingSafety)
	{
		// The kBaseVal is baked into the array.
		return wKingEndgameStaticScores[board.getWhiteKingSquare()];
	}

	// Non-endgame: reward king protection.
	Score score = 0;
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

	return score + kBaseVal;
}

Score BoardEvaluator::getBlackKingScore(const BoardState& board, int8_t numWhiteMajorPieces) const
{
	using namespace scoringConstants;

	if (numWhiteMajorPieces < minNumOpponentMajorPieceRewardKingSafety)
	{
		// The kBaseVal is baked into the array.
		return bKingEndgameStaticScores[board.getBlackKingSquare()];
	}

	// Non-endgame: reward king protection.
	Score score = 0;
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

	return score - kBaseVal;
}

Score BoardEvaluator::getPieceMoveDeltaScore(const BoardState& board, const Move& move,
	const PreMoveInfo& preMoveInfo, const FastSqLookup& lookup) const
{
	assert(canUseGetStaticEvaluationDelta(move));

	switch (move.movingPiece)
	{
		case pieces::wN:
			return wKnightStaticScores[move.toSquare] - wKnightStaticScores[move.fromSquare];
		case pieces::bN:
			return bKnightStaticScores[move.toSquare] - bKnightStaticScores[move.fromSquare];
		case pieces::wB:
			return getWhiteBishopMoveDeltaScore(board, move, lookup);
		case pieces::bB:
			return getBlackBishopMoveDeltaScore(board, move, lookup);
		case pieces::wR:
			return getWhiteRookMoveDeltaScore(board, move, preMoveInfo, lookup);
		case pieces::bR:
			return getBlackRookMoveDeltaScore(board, move, preMoveInfo, lookup);
		case pieces::wQ:
			return wQueenStaticScores[move.toSquare] - wQueenStaticScores[move.fromSquare];
		case pieces::bQ:
			return bQueenStaticScores[move.toSquare] - bQueenStaticScores[move.fromSquare];
		default:
			EngineUtilities::logE("Piece passed to getPieceMoveDeltaScore that cannot be used.");
			return 0;
	}
}

Score BoardEvaluator::getWhiteBishopMoveDeltaScore(const BoardState& board, const Move& move,
	const FastSqLookup& lookup) const
{
	Score score = wBishopStaticScores[move.toSquare] - wBishopStaticScores[move.fromSquare];
	score += getWhiteBishopSquareCoverScore(board, lookup, move.toSquare);
	score -= getWhiteBishopSquareCoverScore(board, lookup, move.fromSquare);
	return score;
}

Score BoardEvaluator::getBlackBishopMoveDeltaScore(const BoardState& board, const Move& move,
	const FastSqLookup& lookup) const
{
	Score score = bBishopStaticScores[move.toSquare] - bBishopStaticScores[move.fromSquare];
	score += getBlackBishopSquareCoverScore(board, lookup, move.toSquare);
	score -= getBlackBishopSquareCoverScore(board, lookup, move.fromSquare);
	return score;
}

Score BoardEvaluator::getWhiteRookMoveDeltaScore(const BoardState& board, const Move& move,
	const PreMoveInfo& preMoveInfo, const FastSqLookup& lookup) const
{
	Score score = wRookStaticScores[move.toSquare] - wRookStaticScores[move.fromSquare];

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

Score BoardEvaluator::getBlackRookMoveDeltaScore(const BoardState& board, const Move& move,
	const PreMoveInfo& preMoveInfo, const FastSqLookup& lookup) const
{
	Score score = bRookStaticScores[move.toSquare] - bRookStaticScores[move.fromSquare];

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
