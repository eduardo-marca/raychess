#pragma once

#include <piece.hpp>
#include <string>

struct Move {
    int from;
    int to;
    bool isCheck;
    bool isCapture;
    bool isAttack;
    bool isCastling;
    bool isEnPassant;
    bool isPromotion;
    Piece promotionPiece;

    std::string toString() {
        return "move(" + std::to_string(from) + ", " + std::to_string(to) + ")";
    }
};
