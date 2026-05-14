#pragma once

#include <movegen.hpp>
#include <move.hpp>
#include <position.hpp>
#include <raylib.h>
#include <vector>
#include <array>

class Game {
private:
    int selectedSquare;
    bool dragging;
    int draggingSquare;
    Vector2 dragPosition;
    Position position;
    MoveGenerator movegen;
    std::vector<Move> legalMoves;
    std::vector<Move> selectedMoves;

    int square_from_mouse(Vector2 mouse) const;
    bool attempt_move(int fromSquare, int toSquare);
    void refresh_legal_moves();
    void refresh_selected_moves();
    const Move* find_legal_move(int fromSquare, int toSquare) const;

public:
    Game();

    void update();

    int getSelectedSquare() const;
    bool isDragging() const;
    int getDraggingSquare() const;
    Vector2 getDragPosition() const;
    PieceColor getSideToMove() const;
    const Board& getBoard() const;
    const std::vector<Move>& getSelectedMoves() const;
};
