#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

Game::Game() : m_numTargets(0), currentCell(CENTRE), dt(0)
{
}

void Game::init()
{
    enemySpawning(MAX_ENEMIES);
    npcs.reserve(MAX_NPCS);
    npcs.push_back(std::make_shared<NPC>(NPC("Fred", {500.0f, 500.0f}, std::make_shared<DefensiveBehaviour>())));
    npcs.push_back(std::make_shared<NPC>(NPC("Maddy", {700.0f, 300.0f}, std::make_shared<OffensiveBehaviour>())));
    npcs.push_back(std::make_shared<NPC>(NPC("Ferdia", {400.0f, 600.0f}, std::make_shared<OffensiveBehaviour>())));

    supplies.reserve(MAX_SUPPLIES);
    supplies.push_back(std::make_shared<Supply>());

    for (auto& npc : npcs)
    {
        npc->init();
    }

    for (auto& supply : supplies)
    {
        supply->init();
    }
}

void Game::draw()
{
    DrawFPS(0, 0);
    player.draw();
    for (Enemy& enemy : enemies)
    {
        enemy.draw();
    }
    for (auto &npc : npcs)
    {
        npc->draw();
    }
    for (auto& supply : supplies)
    {
        supply->draw();
    }

    std::string cellText;
    switch (currentCell)
    {
    case CELLNORTH:
        cellText = "NORTH";
        break;
    case CELLSOUTH:
        cellText = "SOUTH";
        break;
    case CELLEAST:
        cellText = "EAST";
        break;
    case CELLWEST:
        cellText = "WEST";
        break;
    case CENTRE:
        cellText = "CENTRE";
        break;
    default:
        break;
    }

    DrawText(cellText.c_str(), 500, 10, 30, WHITE);

    std::string supplyText;
    supplyText = "Supplies: " + std::to_string(player.currentSupply());
    DrawText(supplyText.c_str(), 10, 50, 20, WHITE);
}

void Game::update()
{
    dt += GetFrameTime();

    player.update();

    if (dt > 0.5f)
    {
        moveCell();
        dt = 0.0f;
    }

    for (Enemy& enemy : enemies)
    {
        enemy.update(player.getPosition());
    }
    collisionCheck();

    if (IsKeyReleased(KEY_R)) { enemySpawning(MAX_ENEMIES); } // DEV KEY
    if (IsKeyReleased(KEY_T)) // DEV KEY
    {
        for (auto &enemy : enemies)
        {
            enemy.kill();
        }
    }
    if (IsKeyReleased(KEY_UP)) { currentCell = CELLNORTH; } // DEV KEY
    if (IsKeyReleased(KEY_DOWN)) { currentCell = CELLSOUTH; } // DEV KEY
    if (IsKeyReleased(KEY_RIGHT)) { currentCell = CELLEAST; } // DEV KEY
    if (IsKeyReleased(KEY_LEFT)) { currentCell = CELLWEST; } // DEV KEY
    if (IsKeyReleased(KEY_SPACE)) { currentCell = CENTRE; } // DEV KEY

    if (currentCell == CENTRE)
    {
        for (auto& enemy : enemies)
        {
            enemy.kill();
        }
    }

    findNPCTarget();

    for (auto& npc : npcs)
    {
        npc->findTarget(m_targetsArray, m_numTargets);
        npc->update(player.getPosition());
    }

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


            for (auto& npc : npcs)
            {
                if (npc->isActive())
                {
                    if (CheckCollisionCircles(npc->getBulletPos(), npc->getBulletRadius(), enemy.getPosition(), enemy.getRadius()))
                    {
                        enemy.damage();
                        npc->resetBullet();
                    }
                }
            }
        }
    }

    for (auto &npc : npcs)
    {
        if (CheckCollisionCircles(player.getPosition(), player.getRadius(), npc->getPosition(), npc->getRadius()))
        {
            npc->startFollowing();
        }
    }

    for (auto& supply : supplies)
    {
        if (supply->isActive())
        {
            if (CheckCollisionCircles(player.getPosition(), player.getRadius(), supply->getPosition(), supply->getRadius()))
            {
                player.addSupply(supply->supplyValue());
                supply->kill();
            }
        }
    }
}

void Game::enemySpawning(int t_num)
{
    if (currentCell != CENTRE)
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

            while (direction == (int)currentCell)
            {
                direction = (Direction)(rand() % 4);
            }

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
}

void Game::supplySpawning(int t_amount)
{
    Vector2 pos;

    for (int i = 0; i < t_amount; i++)
    {
        pos.x = (rand() % SCREEN_WIDTH - 50) + 50;
        pos.y = (rand() % SCREEN_HEIGHT - 50) + 50;

        supplies.at(i)->spawn(pos);
    }
}

void Game::findNPCTarget()
{
    m_numTargets = 0;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].isActive())
        {
            m_targetsArray[m_numTargets] = enemies[i].getPosition();
            m_numTargets++;
        }
    }
}

void Game::moveCell()
{
    if (currentCell == CENTRE)
    {
        if (player.getPosition().x <= 0.0f)
        {
            currentCell = CELLWEST;
            player.setPositionX(SCREEN_WIDTH - player.getRadius());
            enemySpawning(MAX_ENEMIES);
            supplySpawning(1);
        }
        else if (player.getPosition().x >= SCREEN_WIDTH)
        {
            currentCell = CELLEAST;
            player.setPositionX(0.0f);
            enemySpawning(MAX_ENEMIES);
            supplySpawning(1);
        }
        else if (player.getPosition().y <= 0.0f)
        {
            currentCell = CELLNORTH;
            player.setPositionY(SCREEN_HEIGHT);
            enemySpawning(MAX_ENEMIES);
            supplySpawning(1);
        }
        else if (player.getPosition().y >= SCREEN_HEIGHT)
        {
            currentCell = CELLSOUTH;
            player.setPositionY(0.0f);
            enemySpawning(MAX_ENEMIES);
            supplySpawning(1);
        }

        return;
    }

    if (player.getPosition().y <= 0.0f && currentCell == CELLSOUTH)
    {
        currentCell = CENTRE;
        player.setPositionY(SCREEN_HEIGHT - player.getRadius());
    }
    else if (player.getPosition().y >= SCREEN_HEIGHT + player.getRadius() && currentCell == CELLNORTH)
    {
        currentCell = CENTRE;
        player.setPositionY(0);
    }
    else if (player.getPosition().x <= 0.0f && currentCell == CELLEAST)
    {
        currentCell = CENTRE;
        player.setPositionX(SCREEN_WIDTH - player.getRadius());
    }
    else if (player.getPosition().x >= SCREEN_WIDTH && currentCell == CELLWEST)
    {
        currentCell = CENTRE;
        player.setPositionX(0);
    }
}

