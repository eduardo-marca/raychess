#include <raylib.h>

#include <consts.hpp>
#include <board.hpp>
#include <renderer.hpp>

int main(void) {

    InitWindow(screenWidth, screenHeight, "RayChess");
    
    SetTargetFPS(60);
    
    Board board;
    Renderer renderer;

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            renderer.renderBoard(board);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
