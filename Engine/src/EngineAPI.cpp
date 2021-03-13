#include "Engine/EngineAPI.h"

#include "PrivateInclude/Engine.h"
#include "PrivateInclude/BoardState.h"
#include "PrivateInclude/Move.h"

hceEngine::EngineAPI::EngineAPI()
{
    engine = std::make_unique<Engine>();
}

hceEngine::EngineAPI::~EngineAPI()
{
    // This destructor must be explicitly defined in this cpp file for the unique_ptr<Engine>
    // to work properly since Engine is forward delcared.
}

namespace
{
    size_t countMovesRecursive(const Engine& engine, BoardState& board, int32_t depth)
    {
        if (depth == 1)
        {
            return engine.getLegalMoves(board).size();
        }

        const auto moves = engine.getLegalMoves(board);
        size_t num = 0;
        for (const auto& move : moves)
        {
            board.makeMove(move);
            num += countMovesRecursive(engine, board, depth - 1);
            board.unmakeMove(move);
        }
        return num;
    }
}

std::optional<size_t> hceEngine::EngineAPI::getNumLegalMoves(const std::string& FEN, int32_t depth) const
{
    assert(engine != nullptr);

    BoardState board;
    if (!board.initFromFEN(FEN))
    {
        return {};
    }

    return countMovesRecursive(*engine, board, depth);
}
