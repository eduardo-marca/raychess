#include "game.hpp"
#include <consts.hpp>

Game::Game() {
    selectedSquare = -1;
    dragging = false;
    draggingSquare = -1;
    dragPosition = { 0.0f, 0.0f };
    sideToMove = PieceColor::White;
}

void Game::update() {
    refresh_legal_moves();

    Vector2 mouse = GetMousePosition();
    int hovered = square_from_mouse(mouse);

    if (dragging) {
        dragPosition = mouse;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;

            if (hovered != -1 && hovered != draggingSquare) {
                bool moved = attempt_move(draggingSquare, hovered);
                selectedSquare = moved ? -1 : draggingSquare;
            } else {
                selectedSquare = draggingSquare;
            }

            draggingSquare = -1;
        }

        refresh_selected_moves();
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (hovered == -1) {
            selectedSquare = -1;
            return;
        }

        int row = hovered / 8;
        int col = hovered % 8;
        Piece clicked = board.get_piece(row, col);

        if (selectedSquare != -1 && hovered != selectedSquare) {
            if (!isNone(clicked) && getColor(clicked) == sideToMove) {
                selectedSquare = hovered;
                dragging = true;
                draggingSquare = hovered;
                dragPosition = mouse;
            } else {
                bool moved = attempt_move(selectedSquare, hovered);
                if (moved) {
                    selectedSquare = -1;
                }
            }
            return;
        }

        if (!isNone(clicked) && getColor(clicked) == sideToMove) {
            selectedSquare = hovered;
            dragging = true;
            draggingSquare = hovered;
            dragPosition = mouse;
        } else {
            selectedSquare = -1;
        }
    }

    refresh_selected_moves();
}

int Game::getSelectedSquare() const {
    return selectedSquare;
}

bool Game::isDragging() const {
    return dragging;
}

int Game::getDraggingSquare() const {
    return draggingSquare;
}

Vector2 Game::getDragPosition() const {
    return dragPosition;
}

PieceColor Game::getSideToMove() const {
    return sideToMove;
}

const std::vector<Move>& Game::getSelectedMoves() const {
    return selectedMoves;
}

int Game::square_from_mouse(Vector2 mouse) const {
    if (mouse.x < 0 || mouse.y < 0 || mouse.x >= screenWidth || mouse.y >= screenHeight) {
        return -1;
    }

    int col = (int)(mouse.x / square_size);
    int row = (int)(mouse.y / square_size);
    return row * 8 + col;
}

bool Game::attempt_move(int fromSquare, int toSquare) {
    if (fromSquare == toSquare) {
        return false;
    }

    const Move* move = find_legal_move(fromSquare, toSquare);
    if (!move) {
        return false;
    }

    int fromRow = fromSquare / 8;
    int fromCol = fromSquare % 8;
    int toRow = toSquare / 8;
    int toCol = toSquare % 8;

    Piece moving = board.get_piece(fromRow, fromCol);
    if (isNone(moving) || getColor(moving) != sideToMove) {
        return false;
    }

    Piece promotion = move->isPromotion ? move->promotionPiece : Piece::None;
    board.move_piece(fromRow, fromCol, toRow, toCol, promotion);
    sideToMove = (sideToMove == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    return true;
}

void Game::refresh_legal_moves() {
    legalMoves = movegen.generateMoves(board, sideToMove);
}

void Game::refresh_selected_moves() {
    selectedMoves.clear();
    if (selectedSquare == -1) {
        return;
    }

    std::array<int, 64> indexByTarget;
    indexByTarget.fill(-1);

    Piece queen = makePiece(sideToMove, PieceType::Q);

    for (const Move& move : legalMoves) {
        if (move.from != selectedSquare) {
            continue;
        }

        int& slot = indexByTarget[move.to];
        if (slot == -1) {
            selectedMoves.push_back(move);
            slot = static_cast<int>(selectedMoves.size() - 1);
        } else if (move.isPromotion && move.promotionPiece == queen) {
            selectedMoves[slot] = move;
        }
    }
}

const Move* Game::find_legal_move(int fromSquare, int toSquare) const {
    const Move* fallback = nullptr;
    Piece queen = makePiece(sideToMove, PieceType::Q);

    for (const Move& move : legalMoves) {
        if (move.from != fromSquare || move.to != toSquare) {
            continue;
        }

        if (!fallback) {
            fallback = &move;
        }

        if (move.isPromotion && move.promotionPiece == queen) {
            return &move;
        }
    }

    return fallback;
}
