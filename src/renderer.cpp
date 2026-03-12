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
    renderBoard(game.board);
}

void Renderer::renderBoard(Board &board) {
    // Render board squares
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            bool isLightSquare = (row + col) % 2 == 0;
            Color color = isLightSquare ? LIGHT_SQUARE_COLOR : DARK_SQUARE_COLOR;
            DrawRectangle(col * square_size, row * square_size, square_size, square_size, color);
        }
    }

    // Render board pieces
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
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
}
