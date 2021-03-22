#include "StaticEvaluationTests.h"

#include "TestsUtilities.h"
#include "Engine/EngineAPI.h"

#include <string>

namespace
{
	using namespace hceEngine;

	std::string toString(StaticEvaluationResult res)
	{
		switch (res)
		{
			case StaticEvaluationResult::Invalid:
				return "INVALID";
			case StaticEvaluationResult::Equal:
				return "EQUAL";
			case StaticEvaluationResult::WhiteBetter:
				return "WHITE BETTER";
			case StaticEvaluationResult::BlackBetter:
				return "BLACK BETTER";
			default:
				TestsUtilities::logE("Unknown StaticEvaluationResult passed to toString.");
				return "UNKNOWN";
		}
	}

	bool test(const EngineAPI& engine, const std::string& FEN, StaticEvaluationResult expected)
	{
		const StaticEvaluationResult actual = engine.evaluateStatic(FEN);
		if (actual != expected)
		{
			TestsUtilities::logE("Static evaluation test failed for FEN: " + FEN + "\n" +
				"Expected result: " + toString(expected) + " but got: " + toString(actual) + ".");
				return false;
		}

		return true;
	}

	bool testPieceAdvantages(const EngineAPI& engine)
	{
		const std::string wOneLessPawn = "rnbqkbnr/pppppppp/8/8/8/8/PPPPP1PP/RNBQKBNR w KQkq - 0 1";
		if (!test(engine, wOneLessPawn, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string bOneLessPawn = "rnbqkbnr/pp1ppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
		if (!test(engine, bOneLessPawn, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string wOneLessBishop = "rnbqkbnr/p1pppppp/8/1p6/1P6/8/P1PPPPPP/RN1QKBNR w KQkq - 0 1";
		if (!test(engine, wOneLessBishop, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string bOneLessBishop = "rn1qkbnr/p1pppppp/8/1p6/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 1";
		if (!test(engine, bOneLessBishop, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string wOneLessKnight = "rnbqkbnr/p1pppppp/8/1p6/1P6/8/P1PPPPPP/R1BQKBNR w KQkq - 0 1";
		if (!test(engine, wOneLessKnight, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string bOneLessKnight = "r1bqkbnr/p1pppppp/8/1p6/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 1";
		if (!test(engine, bOneLessKnight, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string wOneLessRook = "rnbqkbnr/1ppppppp/8/p7/P7/8/1PPPPPPP/1NBQKBNR w Kkq - 0 1";
		if (!test(engine, wOneLessRook, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string bOneLessRook = "1nbqkbnr/1ppppppp/8/p7/P7/8/1PPPPPPP/RNBQKBNR w KQk - 0 1";
		if (!test(engine, bOneLessRook, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string wOneLessQueen = "rnbqkbnr/1p1ppppp/2p5/p7/P7/2P5/1P1PPPPP/RNB1KBNR w KQkq - 0 1";
		if (!test(engine, wOneLessQueen, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string bOneLessQueen = "rnb1kbnr/1p1ppppp/2p5/p7/P7/2P5/1P1PPPPP/RNBQKBNR w KQkq - 0 1";
		if (!test(engine, bOneLessQueen, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		return true;
	}

	bool testKingProtection(const EngineAPI& engine)
	{
		const std::string wKingToTheSide1 = "rnbqk2r/pppp1ppp/4p3/8/8/4P3/PPPP1PPP/RNBQ1RK1 w - - 0 1";
		if (!test(engine, wKingToTheSide1, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bKingToTheSide1 = "rnbq1rk1/pppp1ppp/4p3/8/8/4P3/PPPP1PPP/RNBQK2R w - - 0 1";
		if (!test(engine, bKingToTheSide1, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wKingToTheSide2 = "rnbqkr2/ppp2ppp/3pp3/8/8/3PP3/PPP2PPP/RNBQ1R1K w - - 0 1";
		if (!test(engine, wKingToTheSide2, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bKingToTheSide2 = "rnbq1r1k/ppp2ppp/3pp3/8/8/3PP3/PPP2PPP/RNBQKR2 w - - 0 1";
		if (!test(engine, bKingToTheSide2, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wKingToTheSide3 = "2r1kbnr/pppq1ppp/3pp3/8/8/3PP3/PPPQ1PPP/K1R2BNR w - - 0 1";
		if (!test(engine, wKingToTheSide3, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bKingToTheSide3 = "k1r2bnr/pppq1ppp/3pp3/8/8/3PP3/PPPQ1PPP/2RK1BNR w - - 0 1";
		if (!test(engine, bKingToTheSide3, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string bothKingsProtected = "rnbq1rk1/pppp1ppp/4p3/8/8/4P3/PPPP1PPP/RNBQ1RK1 w - - 0 1";
		if (!test(engine, bothKingsProtected, hceEngine::StaticEvaluationResult::Equal))
		{
			return false;
		}

		const std::string wKingprotected1 = "rnbq1rk1/pppp1p2/4p1pp/8/8/4P2P/PPPP1PP1/RNBQ1RK1 w - - 0 1";
		if (!test(engine, wKingprotected1, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bKingprotected1 = "rnbq1rk1/pppp2p1/4pp1p/8/8/4PPPP/PPPP4/RNBQ1RK1 w - - 0 1";
		if (!test(engine, bKingprotected1, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wKingprotected2 = "2rk1r2/pppbn1p1/3ppp1p/8/8/3PPP1P/PPPBN1P1/1KR2R2 w - - 0 1";
		if (!test(engine, wKingprotected2, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bKingprotected2 = "1kr2r2/pppbn1p1/3ppp1p/8/8/3PPP1P/PPPBN1P1/2RK1R2 w - - 0 1";
		if (!test(engine, bKingprotected2, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		return true;
	}

	bool testStrategicPositioning(const EngineAPI& engine)
	{
		const std::string startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
		if (!test(engine, startPosition, hceEngine::StaticEvaluationResult::Equal))
		{
			return false;
		}

		const std::string equalPawnAdvance = "rnbqkbnr/p6p/1p4p1/2pppp2/2PPPP2/1P4P1/P6P/RNBQKBNR w KQkq - 0 1";
		if (!test(engine, equalPawnAdvance, hceEngine::StaticEvaluationResult::Equal))
		{
			return false;
		}

		const std::string symmetricComplex = "1kr2r2/pppbn1p1/3ppp1p/8/8/3PPP1P/PPPBN1P1/1KR2R2 w - - 0 1";
		if (!test(engine, symmetricComplex, hceEngine::StaticEvaluationResult::Equal))
		{
			return false;
		}

		const std::string wKnightCloseToCenter = "rnbqkb1r/pppppppp/7n/8/8/5N2/PPPPPPPP/RNBQKB1R w KQkq - 0 1";
		if (!test(engine, wKnightCloseToCenter, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bKnightCloseToCenter = "r1bqkbnr/pppppppp/2n5/8/8/N7/PPPPPPPP/R1BQKBNR w KQkq - 0 1";
		if (!test(engine, bKnightCloseToCenter, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wPawnFurtherAdvanced = "7k/8/P7/7p/8/8/8/K7 w - - 0 1";
		if (!test(engine, wPawnFurtherAdvanced, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bPawnFurtherAdvanced = "7k/8/8/8/P7/7p/8/K7 w - - 0 1";
		if (!test(engine, bPawnFurtherAdvanced, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wRookOpenFile = "1nr2rk1/pppp1ppp/4p3/8/8/1P2P3/PP1P1PPP/1NR2RK1 w - - 0 1";
		if (!test(engine, wRookOpenFile, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bRookOpenFile = "1nr2rk1/pp1p1ppp/1p2p3/8/8/4P3/PPPP1PPP/1NR2RK1 w - - 0 1";
		if (!test(engine, bRookOpenFile, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wRookEndGMoreSqCovered = "4r1k1/pppp1ppp/4p3/4R3/8/1P2P3/PP1P1PPP/6K1 w - - 0 1";
		if (!test(engine, wRookEndGMoreSqCovered, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bRookEndGMoreSqCovered = "6k1/pppp1ppp/4p3/4r3/8/1P2P3/PP1P1PPP/4R1K1 w - - 0 1";
		if (!test(engine, bRookEndGMoreSqCovered, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string bBlockedBisop = "rnbqk1nr/ppp1b2p/3pp1p1/5p2/5P2/3PP1P1/PPP3BP/RNBQK1NR w KQkq - 0 1";
		if (!test(engine, bBlockedBisop, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string wBlockedBisop = "rnbqk1nr/ppp3bp/3pp1p1/5p2/5P2/3PP1P1/PPPB3P/RNBQK1NR w KQkq - 0 1";
		if (!test(engine, wBlockedBisop, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string symmetricBishops = "4k3/5p2/4b3/5p2/5P2/4B3/5P2/4K3 w - - 0 1";
		if (!test(engine, symmetricBishops, hceEngine::StaticEvaluationResult::Equal))
		{
			return false;
		}

		const std::string bBishopHigherSqCover1 = "4k3/4p3/4b3/5p2/5P2/4B3/5P2/4K3 w - - 0 1";
		if (!test(engine, bBishopHigherSqCover1, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wBishopHigherSqCover1 = "4k3/5p2/4b3/5p2/5P2/4B3/4P3/4K3 w - - 0 1";
		if (!test(engine, wBishopHigherSqCover1, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bBishopHigherSqCover2 = "4k3/4p3/4b3/3p1p2/3P1P2/4B3/3P4/4K3 w - - 0 1";
		if (!test(engine, bBishopHigherSqCover2, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wBishopHigherSqCover2 = "4k3/3p4/4b3/3p1p2/3P1P2/4B3/4P3/4K3 w - - 0 1";
		if (!test(engine, wBishopHigherSqCover2, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string wKingEndGCentered = "1k6/8/8/5p2/5P2/3K4/8/8 w - - 0 1";
		if (!test(engine, wKingEndGCentered, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		const std::string bKingEndGCentered = "8/8/4k3/5p2/5P2/8/8/K7 w - - 0 1";
		if (!test(engine, bKingEndGCentered, hceEngine::StaticEvaluationResult::BlackBetter))
		{
			return false;
		}

		const std::string wPawnCentered = "rnb1k1nr/pp1ppp1p/2p5/6p1/4P3/2P5/PP1P1PPP/RNB1K1NR w KQkq - 0 1";
		if (!test(engine, wPawnCentered, hceEngine::StaticEvaluationResult::WhiteBetter))
		{
			return false;
		}

		return true;
	}
}

void StaticEvaluationTests::Run()
{
	hceEngine::EngineAPI engine;
	
	if (!testPieceAdvantages(engine))
	{
		TestsUtilities::log("Static evaluation: piece advantage tests failed.");
		return;
	}

	if (!testKingProtection(engine))
	{
		TestsUtilities::log("Static evaluation: king protection tests failed.");
		return;
	}

	if (!testStrategicPositioning(engine))
	{
		TestsUtilities::log("Static evaluation: strategic positioning tests failed.");
		return;
	}

	TestsUtilities::log("All static evaluation tests finished successfully.");
}
