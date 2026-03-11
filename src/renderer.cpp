#include "renderer.hpp"
#include <consts.hpp>

void Renderer::renderBoard(Board &board) {
    // Render board squares
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            bool isLightSquare = (row + col) % 2 == 0;
            Color color = isLightSquare ? LIGHT_SQUARE_COLOR : DARK_SQUARE_COLOR;
            DrawRectangle(col * square_size, row * square_size, square_size, square_size, color);
        }
    }
}
