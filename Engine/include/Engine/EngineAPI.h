#pragma once

#include "EngineAPIReturnDefinitions.h"

#include <string>
#include <optional>
#include <memory>

class Engine;

namespace hceEngine
{
    class EngineAPI
    {
    public:
        EngineAPI();
        ~EngineAPI();

        /**
        * Uses alpha-beta pruning and quiescence search to find the best move.
        * This is the recommended function to call if maximum performance is wanted given a
        * predetermined depth.
        */
        SearchResult getBestMove(const std::string& FEN, uint8_t depth) const;

        /**
        * Uses the simplest (and slow) minimax algorithm to find the best move given a FEN.
        * Does not utilize any quiescence search and is therefore likely to suffer from the
        * horizon effect. For maximum performace, use the e.g. getBestMove() function instead.
        */
        SearchResult getBestMoveMiniMax(const std::string& FEN, uint8_t depth) const;

        /**
        * Given a FEN, returns a LegalMovesCollection containing all legal moves.
        * If no legal moves are found, the PlayState in the LegalMovesCollection will indicate
        * which side is the winning side, or if it is a draw.
        * Note that the static evaluation of each move contained in the LegalMovesCollection
        * is not set when calling this function, and is always set to zero.
        */
        LegalMovesCollection getLegalMoves(const std::string& FEN) const;

        /**
        * Given a FEN, returns the numbe of legal moves down to a certain depth.
        * For large depths, this function may take a long time to return since the number of moves
        * grows exponentially with the depth.
        */
        std::optional<size_t> getNumLegalMoves(const std::string& FEN, uint8_t depth) const;

        /**
        * Uses the engines internal static board evaluation function to determine the better side.
        * 
        * Important note: the static evaluation functions does NOT consider any "dynamic" aspect
        * of a given position, e.g. whos turn it is, king checks, castle availability etc.
        * It only uses simple heuristics about the position such as which pieces are on the board
        * and their positions, king protection, major piece square coverage and similar things to
        * evaluate the given position. This is mostly for internal use and should not be used as a
        * substitute for actual deep analysis. This can be thought of as analysing the board with
        * a depth of zero.
        */
        StaticEvaluationResult evaluateStatic(const std::string& FEN) const;

    private:
        std::unique_ptr<Engine> engine;
    };
}
