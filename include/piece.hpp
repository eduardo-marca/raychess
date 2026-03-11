#pragma once

enum Piece {
    WK, WQ, WB, WN, WR, WP,
    BK, BQ, BB, BN, BR, BP,
    NPIECE
};

enum PieceType {
    K, Q, B, N, R, P, NTYPE
};

enum PieceColor {
    WT, BL, NCOLOR
};

PieceType inline getType(Piece piece) {
    return (PieceType) (piece == NPIECE ? NTYPE : (piece % 6));
}

PieceColor inline getColor(Piece piece) {
    return (PieceColor) (piece / 6);
}
