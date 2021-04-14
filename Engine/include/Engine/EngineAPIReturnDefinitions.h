#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace hceEngine
{
	enum class StaticEvaluationResult
	{
		Invalid,
		WhiteBetter,
		BlackBetter,
		Equal
	};

	enum class MoveType
	{
		Invalid,
		None,
		Silent,
		Capture,
		Castling,
		EnPassantCapture,
		PawnPromotionSilent,
		PawnPromotionCapture
	};

	struct EngineInfo
	{
		size_t nodesVisited = 0;
		size_t depthsCompletelyCovered = 0;
		size_t maxDepthVisited = 0;
	};

	struct ChessMove
	{
		MoveType type = MoveType::Invalid;
		std::string fromSquare;
		std::string toSquare;
		std::string movingPiece;
		std::string capturedPiece;
		std::string pawnPromotionPiece;
		float positionEvaluation = 0.f;
	};

	struct SearchResult
	{
		ChessMove move;
		EngineInfo engineInfo;
	};

	enum class PlayState
	{
		Invalid,
		Playing,
		WhiteWins,
		BlackWins,
		Draw
	};

	struct LegalMovesCollection
	{
		std::vector<ChessMove> moves;
		PlayState state = PlayState::Invalid;
	};
}
