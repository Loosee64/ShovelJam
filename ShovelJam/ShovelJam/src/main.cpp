#include "raylib.h"
#include "stdio.h"

#include "../include/game.h"

// Specific include for build_web
#if defined(WEB_BUILD)
#include <emscripten/emscripten.h>
#endif

const int screenWidth = 800;
const int screenHeight = 600;

void GameLoop(void);

Game game;

int main(void)
{

    InitWindow(screenWidth, screenHeight, "Raylib StarterKit GPPI");

    game.init();

#if defined(WEB_BUILD)
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Call GameLoop
        GameLoop();
    }
#endif


    CloseWindow();

    return 0;
}

int counter = 0;
char message[11];

void GameLoop(void)
{
    BeginDrawing();

    // Update Game Data
    // Should be outside BeginDrawing(); and EndDrawing();
    game.update();

    ClearBackground(BLACK);

    // Draw the Game Objects
    game.draw();

    counter++;

    EndDrawing();
}
