#include "raylib.h"
#include "stdio.h"
#include "globals.h"
#include "random"

#include "../include/game.h"

// Specific include for build_web
#if defined(WEB_BUILD)
#include <emscripten/emscripten.h>
#endif


void GameLoop(void);

Game game;

int main(void)
{
    srand(time(nullptr));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Shovel Jam 2025");

    game.init();

#if defined(WEB_BUILD)
    emscripten_set_main_loop(GameLoop, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Call GameLoop
        GameLoop();

        if (exitWindow)
        {
            break;
        }
    }
#endif

    CloseAudioDevice();
    CloseWindow();

    return 0;
}


void GameLoop(void)
{
    BeginDrawing();
    game.update();

    game.draw();

    EndDrawing();
}
