#include "board.hpp"

Board::Board() {
    static const Piece initial[8][8] = {
        {Piece::BR, Piece::BN, Piece::BB, Piece::BQ, Piece::BK, Piece::BB, Piece::BN, Piece::BR},
        {Piece::BP, Piece::BP, Piece::BP, Piece::BP, Piece::BP, Piece::BP, Piece::BP, Piece::BP},
        {Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None},
        {Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None},
        {Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None},
        {Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None, Piece::None},
        {Piece::WP, Piece::WP, Piece::WP, Piece::WP, Piece::WP, Piece::WP, Piece::WP, Piece::WP},
        {Piece::WR, Piece::WN, Piece::WB, Piece::WQ, Piece::WK, Piece::WB, Piece::WN, Piece::WR}
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
                return makePiece(static_cast<PieceColor>(color), static_cast<PieceType>(type));
            }
        }
    }

    return Piece::None;
}

void Board::set_piece(Piece piece, int row, int col) {
    if (isNone(piece)) {
        return;
    }

    int color = to_index(getColor(piece));
    int type = to_index(getType(piece));
    bitboards[color][type] |= square_mask(row, col);
}
