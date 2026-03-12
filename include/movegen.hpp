#pragma once

#include <board.hpp>
#include <move.hpp>
#include <cstdint>
#include <vector>

class MoveGenerator {
public:
    using Bitboard = std::uint64_t;

    static constexpr int CASTLE_WHITE_KING = 1 << 0;
    static constexpr int CASTLE_WHITE_QUEEN = 1 << 1;
    static constexpr int CASTLE_BLACK_KING = 1 << 2;
    static constexpr int CASTLE_BLACK_QUEEN = 1 << 3;

    std::vector<Move> generateMoves(const Board& board, PieceColor color, int castlingRights);

private:
    // Lookup tables (pre-computed)
    static Bitboard knight_attacks[64];
    static Bitboard king_attacks[64];

    Bitboard generateRookMoves(int square, Bitboard occupied);
    Bitboard generateBishopMoves(int square, Bitboard occupied);

    Bitboard generatePawnAttacks(int square, PieceColor color);
    Bitboard generatePawnPushes(int square, PieceColor color, Bitboard occupied);
    Bitboard generateAttacks(const Board& board, PieceColor color);

    // Helper functions
    static inline int square_index(int row, int col) { return row * 8 + col; }
    static inline int bit_position(Bitboard b) { return __builtin_ctzll(b); }
    static inline Bitboard isolate_lsb(Bitboard b) { return b & -b; }
};
