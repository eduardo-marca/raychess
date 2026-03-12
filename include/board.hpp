#pragma once

#include <piece.hpp>
#include <cstdint>

class Board {
    private:
        using Bitboard = std::uint64_t;
        Bitboard bitboards[2][6]{};

        static inline int square_index(int row, int col) {
            return row * 8 + col;
        }

        static inline Bitboard square_mask(int row, int col) {
            return Bitboard(1) << square_index(row, col);
        }

        void clear_square(int row, int col);
        void set_piece(Piece piece, int row, int col);

    public:
        Board();

        Piece get_piece(int row, int col) const;
        void move_piece(int from_row, int from_col, int to_row, int to_col, Piece promotionPiece = Piece::None);
};
