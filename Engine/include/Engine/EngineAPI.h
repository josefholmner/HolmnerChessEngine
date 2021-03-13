#pragma once

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

        std::optional<size_t> getNumLegalMoves(const std::string& FEN, int32_t depth) const;

    private:
        std::unique_ptr<Engine> engine;
    };
}
