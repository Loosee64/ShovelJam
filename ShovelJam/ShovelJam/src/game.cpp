#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

void Game::init()
{
    player.init();
}

void Game::draw()
{
    DrawFPS(0, 0);
    player.draw();
}

void Game::update()
{
    player.update();
}
