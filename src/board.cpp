#include "board.hpp"

Board::Board() {
    static const Piece initial[8][8] = {
        {BR, BN, BB, BQ, BK, BB, BN, BR},
        {BP, BP, BP, BP, BP, BP, BP, BP},
        {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
        {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
        {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
        {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
        {WP, WP, WP, WP, WP, WP, WP, WP},
        {WR, WN, WB, WQ, WK, WB, WN, WR}
    };

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            set_piece(initial[row][col], row, col);
        }
    }
}

Piece Board::get_piece(int row, int col) {
    Bitboard mask = square_mask(row, col);

    for (int color = 0; color < 2; color++) {
        for (int type = 0; type < 6; type++) {
            if (bitboards[color][type] & mask) {
                return (Piece)(color * 6 + type);
            }
        }
    }

    return NPIECE;
}

void Board::set_piece(Piece piece, int row, int col) {
    if (piece == NPIECE) {
        return;
    }

    int color = (int)getColor(piece);
    int type = (int)getType(piece);
    bitboards[color][type] |= square_mask(row, col);
}
