#include <raylib.h>

#include <consts.hpp>
#include <game.hpp>
#include <renderer.hpp>

int main(void) {

    InitWindow(screenWidth, screenHeight, "RayChess");
    
    SetTargetFPS(60);
    
    Game game;
    Renderer renderer;

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
