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

Piece Board::get_piece(int row, int col) const {
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

void Board::move_piece(int from_row, int from_col, int to_row, int to_col, Piece promotionPiece) {
    if (from_row == to_row && from_col == to_col) {
        return;
    }

    Piece piece = get_piece(from_row, from_col);
    if (isNone(piece)) {
        return;
    }

    clear_square(from_row, from_col);
    clear_square(to_row, to_col);
    Piece finalPiece = isNone(promotionPiece) ? piece : promotionPiece;
    set_piece(finalPiece, to_row, to_col);
}

void Board::remove_piece(int row, int col) {
    clear_square(row, col);
}

void Board::set_piece(Piece piece, int row, int col) {
    if (isNone(piece)) {
        return;
    }

    int color = to_index(getColor(piece));
    int type = to_index(getType(piece));
    bitboards[color][type] |= square_mask(row, col);
}

void Board::clear_square(int row, int col) {
    Bitboard mask = square_mask(row, col);
    for (int color = 0; color < 2; color++) {
        for (int type = 0; type < 6; type++) {
            bitboards[color][type] &= ~mask;
        }
    }
}
