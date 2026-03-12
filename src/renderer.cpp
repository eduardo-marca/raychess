#include "renderer.hpp"
#include <consts.hpp>
#include <iostream>

Renderer::Renderer() {
    piecesTextures = LoadTexture("src/imgs/chess_pieces.png");

    float w = piecesTextures.width / 6.0f;
    float h = piecesTextures.height / 2.0f;
    for(int t = 0; t < 6; t++) {
        for(int c = 0; c < 2; c++) {
            piecesRecs[t][c] = {
                .x = t * w,
                .y = c * h,
                .width = w,
                .height = h
            };
        }
    }
}

void Renderer::renderGame(Game &game) {
    renderBoard(game);
}

void Renderer::renderBoard(Game &game) {
    Board &board = game.board;

    // Render board squares
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            bool isLightSquare = (row + col) % 2 == 0;
            Color color = isLightSquare ? LIGHT_SQUARE_COLOR : DARK_SQUARE_COLOR;
            DrawRectangle(col * square_size, row * square_size, square_size, square_size, color);
        }
    }

    int selectedSquare = game.getSelectedSquare();
    if (selectedSquare != -1) {
        int selRow = selectedSquare / 8;
        int selCol = selectedSquare % 8;
        Color highlight = { 255, 255, 0, 80 };
        DrawRectangle(selCol * square_size, selRow * square_size, square_size, square_size, highlight);
    }

    const std::vector<Move>& selectedMoves = game.getSelectedMoves();
    Color moveDot = { 30, 144, 255, 140 };
    Color captureOutline = { 200, 60, 60, 180 };

    for (const Move& move : selectedMoves) {
        if (move.isCapture) {
            continue;
        }
        int row = move.to / 8;
        int col = move.to % 8;
        Vector2 center = {
            col * square_size + square_size * 0.5f,
            row * square_size + square_size * 0.5f
        };
        DrawCircleV(center, square_size * 0.18f, moveDot);
    }

    // Render board pieces
    bool dragging = game.isDragging();
    int draggingSquare = dragging ? game.getDraggingSquare() : -1;

    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            if (dragging && (row * 8 + col) == draggingSquare) {
                continue;
            }

            Piece piece = board.get_piece(row, col);
            if (isNone(piece)) continue;

            PieceType type = getType(piece);
            PieceColor color = getColor(piece);
            int typeIndex = to_index(type);
            int colorIndex = to_index(color);
            Vector2 pos = { (float) col * square_size, (float) row * square_size };
            Rectangle src = piecesRecs[typeIndex][colorIndex];
            Rectangle dst = { pos.x, pos.y, (float)square_size, (float)square_size };
            Vector2 origin = { 0.0f, 0.0f };
            DrawTexturePro(piecesTextures, src, dst, origin, 0.0f, WHITE);
        }
    }

    for (const Move& move : selectedMoves) {
        if (!move.isCapture) {
            continue;
        }
        int row = move.to / 8;
        int col = move.to % 8;
        Rectangle rect = {
            col * square_size + 4.0f,
            row * square_size + 4.0f,
            square_size - 8.0f,
            square_size - 8.0f
        };
        DrawRectangleLinesEx(rect, 3.0f, captureOutline);
    }

    if (dragging && draggingSquare != -1) {
        int row = draggingSquare / 8;
        int col = draggingSquare % 8;
        Piece piece = board.get_piece(row, col);
        if (!isNone(piece)) {
            PieceType type = getType(piece);
            PieceColor color = getColor(piece);
            int typeIndex = to_index(type);
            int colorIndex = to_index(color);
            Rectangle src = piecesRecs[typeIndex][colorIndex];
            Vector2 dragPos = game.getDragPosition();
            Rectangle dst = {
                dragPos.x - square_size * 0.5f,
                dragPos.y - square_size * 0.5f,
                (float)square_size,
                (float)square_size
            };
            Vector2 origin = { 0.0f, 0.0f };
            DrawTexturePro(piecesTextures, src, dst, origin, 0.0f, WHITE);
        }
    }
}
