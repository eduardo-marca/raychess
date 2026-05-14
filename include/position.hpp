#pragma once

#include <board.hpp>
#include <move.hpp>
#include <movegen.hpp>
#include <vector>

class Position {
private:
    void updateCastlingRights(int fromSquare, int toSquare, Piece moving, Piece captured);

public:
    Board board;
    PieceColor sideToMove;
    int castlingRights;
    int enPassantSquare;

    Position();

    std::vector<Move> generateLegalMoves(MoveGenerator& movegen) const;
    bool applyMove(const Move& move);
};
