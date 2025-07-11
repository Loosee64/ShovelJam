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
    for (Enemy& enemy : enemy)
    {
        enemy.draw();
    }
}

void Game::update()
{
    player.update();
    for (Enemy& enemy : enemy)
    {
        enemy.update();
    }
    collisionCheck();
}

void Game::collisionCheck()
{
    for (Enemy& enemy : enemy)
    {
        if (enemy.isActive())
        {
            if (CheckCollisionCircles(player.getBulletPos(), player.getRadius(), enemy.getPosition(), enemy.getRadius()))
            {
                enemy.kill();
                player.resetBullet();
            }
        }
    }
}
