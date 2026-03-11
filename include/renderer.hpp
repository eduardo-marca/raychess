#pragma once

#include <board.hpp>
#include <raylib.h>

class Renderer {
    private:
        Texture2D piecesTextures;
        Rectangle piecesRecs[6][2];

    public:
        Renderer();

        void renderBoard(Board& board);
};
