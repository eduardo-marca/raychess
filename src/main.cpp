#include <raylib.h>

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {

    InitWindow(screenWidth, screenHeight, "Raylib Template");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Raylib Template", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
