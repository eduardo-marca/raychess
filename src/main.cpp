#include <raylib.h>

#include <consts.hpp>
#include <renderer.hpp>

int main(void) {

    Renderer renderer;
    Board board;

    InitWindow(screenWidth, screenHeight, "RayChess");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            renderer.renderBoard(board);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
