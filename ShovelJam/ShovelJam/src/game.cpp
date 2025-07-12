#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

void Game::init()
{
    enemySpawning(MAX_ENEMIES);
}

void Game::draw()
{
    DrawFPS(0, 0);
    player.draw();
    for (Enemy& enemy : enemies)
    {
        enemy.draw();
    }
}

void Game::update()
{
    player.update();
    for (Enemy& enemy : enemies)
    {
        enemy.update(player.getPosition());
    }
    collisionCheck();

    if (IsKeyReleased(KEY_R)) { enemySpawning(MAX_ENEMIES); }
}

void Game::collisionCheck()
{
    for (Enemy& enemy : enemies)
    {
        if (enemy.isActive())
        {
            if (player.getActiveBullet() > -1)
            {
                if (CheckCollisionCircles(player.getBulletPos(), player.getBulletRadius(), enemy.getPosition(), enemy.getRadius()))
                {
                    enemy.damage();
                    player.resetBullet();
                }
            }
            if (CheckCollisionCircles(player.getPosition(), player.getRadius(), enemy.getPosition(), enemy.getRadius()))
            {
                player.damage();
                enemy.recoil(20.0f);
            }
        }
    }
}

void Game::enemySpawning(int t_num)
{
    Vector2 start = { 0.0f, 0.0f };
    Direction direction;
    int limit;

    if (t_num < MAX_ENEMIES) // Loop once, spawn specific enemy
    {
        limit = 1;
    }
    else // Spawn all enemies
    {
        limit = MAX_ENEMIES;
    }

    for (int i = 0; i < limit; i++)
    {
        direction = (Direction)(rand() % 4);

        switch (direction)
        {
        case NORTH:
            start.x = (rand() % (SCREEN_WIDTH - 50)) + 50;
            start.y = 20;
            break;
        case SOUTH:
            start.x = (rand() % (SCREEN_WIDTH - 50)) + 50;
            start.y = SCREEN_HEIGHT + 20;
            break;
        case EAST:
            start.x = SCREEN_WIDTH + 20;
            start.y = (rand() % (SCREEN_HEIGHT - 50)) + 50;
            break;
        case WEST:
            start.x = -20.0f;
            start.y = (rand() % (SCREEN_HEIGHT - 50)) + 50;
            break;
        default:
            break;
        }

        if (t_num == MAX_ENEMIES)
        {
            enemies[i].spawn(start);
        }
        else if (t_num < MAX_ENEMIES)
        {
            enemies[t_num].spawn(start);
        }
    }
}
