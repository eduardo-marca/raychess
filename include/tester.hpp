#pragma once

#include <movegen.hpp>
#include <position.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

class Tester {
private:
    MoveGenerator movegen;
    Position rootPosition;

    std::uint64_t perft(const Position& position, int depth);
    static std::string formatMove(const Move& move);

public:
    Tester();

    std::uint64_t countGames(int depth = 1);
    std::vector<std::pair<std::string, std::uint64_t>> divide(int depth);
    std::uint64_t expectedCount(int depth) const;
    bool verifyDepth(int depth);
};
