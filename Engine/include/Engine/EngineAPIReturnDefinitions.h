#pragma once

#include <cstdint>
#include <string>

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
		size_t nodesVisited;
		size_t depthsCompletelyCovered;
		size_t maxDepthVisited;
	};

	struct ChessMove
	{
		MoveType type = MoveType::Invalid;
		std::string fromSquare;
		std::string toSquare;
		std::string movingPiece;
		std::string capturedPiece;
		std::string pawnPromotionPiece;
		int32_t positionEvaluation = 0;
	};

	struct SearchResult
	{
		ChessMove bestMove;
		EngineInfo engineInfo;
	};
}
