#pragma once

#include <board.hpp>
#include <movegen.hpp>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

class Tester {
private:
    struct Position {
        Board board;
        PieceColor sideToMove;
        int castlingRights;
        int enPassantSquare;
    };

    MoveGenerator movegen;
    Position rootPosition;

    std::vector<Move> generateMoves(const Position& position);
    void applyMove(Position& position, const Move& move);
    void updateCastlingRights(Position& position, int fromSquare, int toSquare, Piece moving, Piece captured);
    std::uint64_t perft(const Position& position, int depth);
    std::string formatMove(const Move& move);

public:
    Tester();

    std::uint64_t countGames(int depth = 1);
    std::vector<std::pair<std::string, std::uint64_t>> divide(int depth);
    std::uint64_t expectedCount(int depth) const;
    bool verifyDepth(int depth);
};
