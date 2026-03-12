#pragma once

#include <board.hpp>
#include <raylib.h>

class Game {
private:
    int selectedSquare;
    bool dragging;
    int draggingSquare;
    Vector2 dragPosition;
    PieceColor sideToMove;

    int square_from_mouse(Vector2 mouse) const;
    bool attempt_move(int fromSquare, int toSquare);

public:
    Board board;

    Game();

    void update();

    int getSelectedSquare() const;
    bool isDragging() const;
    int getDraggingSquare() const;
    Vector2 getDragPosition() const;
    PieceColor getSideToMove() const;
};
