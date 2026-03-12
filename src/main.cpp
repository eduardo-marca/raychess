#include <raylib.h>

#include <consts.hpp>
#include <game.hpp>
#include <renderer.hpp>
#include <movegen.hpp>
#include <iostream>

int main(void) {

    InitWindow(screenWidth, screenHeight, "RayChess");
    
    SetTargetFPS(60);
    
    Game game;
    Renderer renderer;
    MoveGenerator movegen;

    while (!WindowShouldClose()) {
        game.update();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            renderer.renderGame(game);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
