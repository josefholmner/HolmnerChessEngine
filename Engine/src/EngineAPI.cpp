#include "Engine/EngineAPI.h"

#include "PrivateInclude/BoardState.h"

std::optional<int32_t> hceEngine::EngineAPI::getNumLegalMoves(const std::string& FEN, int32_t depth) const
{
    BoardState board;
    if (!board.initFromFEN(FEN))
    {
        return {};
    }

    // TODO: implement.
    return 0;
}
