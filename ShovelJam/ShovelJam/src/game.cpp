#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

void Game::init()
{
}

void Game::draw()
{
    DrawFPS(0, 0);
    player.draw();
    enemy.draw();
}

void Game::update()
{
    player.update();
    enemy.update();
    collisionCheck();
}

void Game::collisionCheck()
{
    if (CheckCollisionCircles(player.getBulletPos(), player.getRadius(), enemy.getPosition(), enemy.getRadius()))
    {
        enemy.kill();
        player.resetBullet();
    }
}
