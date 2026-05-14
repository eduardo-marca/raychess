#include <raylib.h>

#include <consts.hpp>
#include <game.hpp>
#include <renderer.hpp>
#include <tester.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc > 1) {
        if (argc != 3) {
            std::cerr << "Usage: ./bin/RayChess [--perft DEPTH | --divide DEPTH | --verify DEPTH]\n";
            return 1;
        }

        std::string mode = argv[1];
        int depth = std::stoi(argv[2]);
        Tester tester;

        if (mode == "--perft") {
            std::cout << tester.countGames(depth) << '\n';
            return 0;
        }

        if (mode == "--divide") {
            auto results = tester.divide(depth);
            std::uint64_t total = 0;
            for (const auto& result : results) {
                std::cout << result.first << ": " << result.second << '\n';
                total += result.second;
            }
            std::cout << "total: " << total << '\n';
            return 0;
        }

        if (mode == "--verify") {
            std::uint64_t actual = tester.countGames(depth);
            std::uint64_t expected = tester.expectedCount(depth);
            if (expected == 0 && depth != 0) {
                std::cerr << "No built-in reference count for depth " << depth << ".\n";
                return 1;
            }

            std::cout << "depth " << depth
                      << " actual=" << actual
                      << " expected=" << expected
                      << (actual == expected ? " OK" : " MISMATCH")
                      << '\n';
            return actual == expected ? 0 : 1;
        }

        std::cerr << "Usage: ./bin/RayChess [--perft DEPTH | --divide DEPTH | --verify DEPTH]\n";
        return 1;
    }

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
