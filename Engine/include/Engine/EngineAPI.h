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
        * Given a FEN, returns the numbe of legal moves down to a certain depth.
        * For large depths, this function may take a long time to return since the number of moves
        * grows exponentially with the depth.
        */
        std::optional<size_t> getNumLegalMoves(const std::string& FEN, uint8_t depth) const;

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

        /**
        * Mostly for internal use. Does the same thing as getBestMove() but skips the quiescence
        * search at each leaf node. This will be faster than the getBestMove() function but will
        * likely suffer from serious strategical errors during play. I.e. the regular getBestMove()
        * function will perform better than this function in a real game, even though this function
        * may reach a deeper search depth given a specific amount of time. It is guaranteed to give
        * the same result as getBestMoveMiniMax() but is faster.
        */
        SearchResult getBestMoveNoQuiescence(const std::string& FEN, uint8_t depth) const;

    private:
        std::unique_ptr<Engine> engine;
    };
}
