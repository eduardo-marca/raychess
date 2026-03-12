#pragma once

#include <cstdint>

enum class Piece : std::uint8_t {
    WK, WQ, WB, WN, WR, WP,
    BK, BQ, BB, BN, BR, BP,
    None
};

enum class PieceType : std::uint8_t {
    K, Q, B, N, R, P, None
};

enum class PieceColor : std::uint8_t {
    White, Black, None
};

static constexpr std::uint8_t PIECES_PER_COLOR = 6;

constexpr inline bool isNone(Piece piece) {
    return piece == Piece::None;
}

constexpr inline PieceType getType(Piece piece) {
    return isNone(piece)
        ? PieceType::None
        : static_cast<PieceType>(static_cast<std::uint8_t>(piece) % PIECES_PER_COLOR);
}

constexpr inline PieceColor getColor(Piece piece) {
    return isNone(piece)
        ? PieceColor::None
        : static_cast<PieceColor>(static_cast<std::uint8_t>(piece) / PIECES_PER_COLOR);
}

constexpr inline Piece makePiece(PieceColor color, PieceType type) {
    if (color == PieceColor::None || type == PieceType::None) {
        return Piece::None;
    }

    return static_cast<Piece>(
        static_cast<std::uint8_t>(color) * PIECES_PER_COLOR
        + static_cast<std::uint8_t>(type)
    );
}
