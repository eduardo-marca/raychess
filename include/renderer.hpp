#pragma once

#include <board.hpp>
#include <game.hpp>
#include <raylib.h>

class Renderer {
    private:
        Texture2D piecesTextures;
        Rectangle piecesRecs[6][2];

    public:
        Renderer();

        void renderGame(Game& game);
        void renderBoard(Game& game);
};
