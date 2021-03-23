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
		int32_t positionEvaluation;
		size_t leafNodesVisited;
		size_t depthsCompletelyCovered;
		size_t maxDepthVisited;
	};

	struct ChessMove
	{
		MoveType type;
		std::string fromSquare;
		std::string toSquare;
		std::string movingPiece;
		std::string capturedPiece;
		std::string pawnPromotionPiece;
	};

	struct SearchResult
	{
		ChessMove bestMove;
		EngineInfo engineInfo;
	};
}
