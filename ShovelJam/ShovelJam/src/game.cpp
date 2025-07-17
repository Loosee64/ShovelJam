#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

Game::Game() : m_numTargets(0), currentCell(CENTRE), dt(0), wavedt(0), enemyKill(0), waveComplete(false), m_numInVillage(0), canInteract(true), time(DAYCYCLE::DAY),
                timedt(0.0f), m_dayCount(1), m_dayText(""), m_raid(false), m_timeText(""), m_gameState(GameState::TITLE), m_dailySuppliesGained(0), m_dailySuppliesConsumed(0),
                m_dailyVillagers(0), m_happiness(0), m_dailyHappiness(0)
{
}

void Game::init()
{
    m_ground = LoadTexture("ASSETS/Spritesheets/ground_spritesheet.png");
    m_scrolling1 = { -SCREEN_WIDTH, 0 };
    m_scrolling2 = { 0, 0 };

    buttons.push_back(std::make_shared<Button>(Button("Play", 300.0f)));
    buttons.push_back(std::make_shared<Button>(Button("Credits", 400.0f)));

    enemySpawning(MAX_ENEMIES);

    village.reserve(MAX_NPCS);

    party.reserve(MAX_PARTY);
    village.push_back(std::make_shared<NPC>(NPC("Barry", { 500.0f, 400.0f }, std::make_shared<OffensiveBehaviour>(), 8)));
    
    supplies.reserve(MAX_SUPPLIES);
    supplies.push_back(std::make_shared<Supply>());

    buildings.reserve(MAX_BUILDINGS);
    buildings.push_back(std::make_shared<Building>(std::make_shared<SupplyShed>(), 2));
    buildings.push_back(std::make_shared<Building>(std::make_shared<Bunks>(), 1));
    buildings.push_back(std::make_shared<Building>(std::make_shared<Barracks>(), 0));
    buildings.push_back(std::make_shared<Building>(std::make_shared<Saloon>(), 1));
    buildings.push_back(std::make_shared<Building>(std::make_shared<SleepingBag>(), 0));

    player.init();

    for (auto& enemy : enemies)
    {
        enemy.init();
    }

    for (auto& npc : party)
    {
        npc->init();
    }

    for (auto& npc : village)
    {
        npc->resetTarget();
    }

    for (auto& supply : supplies)
    {
        supply->init();
    }

    float offset = 0.0f;

    for (auto& building : buildings)
    {
        building->init();

        if (building->getName() == "Sleeping Bag")
        {
            building->spawn({ 400.0f, 500.0f });
            building->forceComplete();
            building->disableText();
            continue;
        }
        if (building->getName() == "Saloon")
        {
            building->spawn({ 800.0f, 600.0f });
            continue;
        }

        building->spawn({ 200.0f + offset, 200.0f });
        offset += 300.0f;
    }

    m_swarmArea.width = 40.0f;
    m_swarmArea.height = 40.0f;
    m_swarmArea.x = 1000.0f;
    m_swarmArea.y = 1000.0f;

    m_healthBar = { 15, 20, 150, 20 };
    m_healthOutline = { 10, 15, 160, 30 };

    swarmSpawning();
}

void Game::reset()
{
    currentCell = CENTRE;

    m_dayCount = 1;
    m_raid = false;
    timedt = 0.0f;
    time = DAYCYCLE::DAY;
    dt = 0;
    wavedt = 0;
    m_numInVillage = 0;
    enemyKill = 0;

    player.reset();
    village.clear();
    party.clear();
    barracks.clear();

    buildings.push_back(std::make_shared<Building>(std::make_shared<SleepingBag>(), 0));
    buildings.back()->init();
    m_effectiveMaxBuildings++;

    for (auto& building : buildings)
    {
        if (building->getName() == "Sleeping Bag")
        {
            building->spawn({ 400.0f, 500.0f });
            building->forceComplete();
            building->disableText();
            continue;
        }

        building->reset();
    }

    swarmSpawning();
}

void Game::draw()
{
    ClearBackground(BLACK);

    switch (m_gameState)
    {
    case GameState::TITLE:
        titleDraw();
        break;
    case GameState::CREDITS:
        creditsDraw();
        break;
    case GameState::GAMEPLAY:
        gameplayDraw();
        break;
    case GameState::GAMEOVER:
        gameOverDraw();
        break;
    case GameState::ENDOFDAY:
        endOfDayDraw();
        break;
    default:
        break;
    }
}

void Game::update()
{
    if (IsKeyReleased(KEY_F1)) { m_gameState = GameState::TITLE; }
    if (IsKeyReleased(KEY_F2)) { m_gameState = GameState::CREDITS; }
    if (IsKeyReleased(KEY_F3)) { m_gameState = GameState::GAMEPLAY; }
    if (IsKeyReleased(KEY_F4)) { m_gameState = GameState::GAMEOVER; }
    if (IsKeyReleased(KEY_F5)) { m_gameState = GameState::ENDOFDAY; }

    switch (m_gameState)
    {
    case GameState::TITLE:
        buttons.at(0)->setValues("Play", 300.0f);
        buttons.at(1)->setValues("Credits", 400.0f);
        titleUpdate();
        break;
    case GameState::CREDITS:
        buttons.at(0)->setValues("Done", 600.0f);
        buttons.at(1)->setValues("", 10000.0f);
        creditsUpdate();
        break;
    case GameState::GAMEPLAY:
        gameplayUpdate();
        break;
    case GameState::GAMEOVER:
        buttons.at(0)->setValues("Try Again", 400.0f);
        buttons.at(1)->setValues("", 10000.0f);
        gameOverUpdate();
        break;
    case GameState::ENDOFDAY:
        buttons.at(0)->setValues("Done", 600.0f);
        buttons.at(1)->setValues("", 10000.0f);
        endOfDayUpdate();
        break;
    default:
        break;
    }
}

void Game::titleUpdate()
{
    screenScrolling();
    for (auto& button : buttons)
    {
        if (button->checkCollision(GetMousePosition()) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (button->getTitle() == "Play")
            {
                m_gameState = GameState::GAMEPLAY;
            }
            else if (button->getTitle() == "Credits")
            {
                m_gameState = GameState::CREDITS;
            }
        }
    }
}

void Game::titleDraw()
{
    DrawTexturePro(m_ground, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling1.x, m_scrolling1.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);
    DrawTexturePro(m_ground, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling2.x, m_scrolling2.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);

    for (auto& button : buttons)
    {
        button->draw();
    }

    std::string titleText = "title";
    DrawText(titleText.c_str(), 0, 0, 40, WHITE);
}

void Game::creditsDraw()
{
    DrawTexturePro(m_ground, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling1.x, m_scrolling1.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);
    DrawTexturePro(m_ground, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling2.x, m_scrolling2.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);

    for (auto& button : buttons)
    {
        button->draw();
    }

    std::string titleText = "credits";
    DrawText(titleText.c_str(), 0, 0, 40, WHITE);
}

void Game::creditsUpdate()
{
    screenScrolling();

    for (auto& button : buttons)
    {
        if (button->checkCollision(GetMousePosition()) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (button->getTitle() == "Done")
            {
                m_gameState = GameState::TITLE;
            }
        }
    }
}

void Game::gameplayUpdate()
{
    if (!player.isAlive())
    {
        m_gameState = GameState::GAMEOVER;
    }
    else
    {
        m_healthBar.width = (player.getHealth() / 5.0f) * 150.0f;
    }

    dt += GetFrameTime();

    dayCycle(false);

    player.update();

    if (dt > 0.5f)
    {
        moveCell();
        dt = 0.0f;
        canInteract = true;
    }

    for (Enemy& enemy : enemies)
    {
        enemy.assignTarget(findNearestTarget(enemy, party));
        if (m_raid)
        {
            enemy.assignTarget(findNearestTarget(enemy, village));
            enemy.assignTarget(findNearestTarget(enemy, barracks));
        }
        enemy.update();
    }

    collisionCheck();

    disableSleepingBag();

    if (IsKeyReleased(KEY_R)) { enemySpawning(MAX_ENEMIES); } // DEV KEY
    if (IsKeyReleased(KEY_T)) // DEV KEY
    {
        for (auto& enemy : enemies)
        {
            enemy.kill();
        }
    }
    if (IsKeyReleased(KEY_UP)) { currentCell = CELLNORTH; } // DEV KEY
    if (IsKeyReleased(KEY_DOWN)) { currentCell = CELLSOUTH; } // DEV KEY
    if (IsKeyReleased(KEY_RIGHT)) { currentCell = CELLEAST; } // DEV KEY
    if (IsKeyReleased(KEY_LEFT)) { currentCell = CELLWEST; } // DEV KEY
    if (IsKeyReleased(KEY_SPACE)) { currentCell = CENTRE; } // DEV KEY

    if (currentCell == CENTRE && !m_raid)
    {
        for (auto& enemy : enemies)
        {
            enemy.kill();
        }
    }

    findNPCTarget();

    for (auto& npc : party)
    {
        if (!npc->isBuilding())
        {
            npc->findTarget(m_targetsArray, m_numTargets);
        }
        npc->update(player.getPosition());
        npc->updateHappiness(m_happiness);
    }

    if (currentCell == CENTRE)
    {
        for (auto& building : buildings)
        {
            building->update();
        }

        for (auto& npc : village)
        {
            if (!m_raid && !npc->isBuilding())
            {
                npc->passive();
                npc->updateHappiness(m_happiness);
                if (npc->isDeserter())
                {
                    village.clear();
                }
            }
            else
            {
                if (!npc->isBuilding())
                {
                    npc->findTarget(m_targetsArray, m_numTargets);
                }
                npc->update({ 0.0f, 0.0f });
            }
        }

        if (m_raid)
        {
            for (auto& npc : barracks)
            {
                npc->findTarget(m_targetsArray, m_numTargets);
                npc->update({ 0.0f, 0.0f });
                npc->updateHappiness(m_happiness);
            }
        }
    }
    else
    {
        if (m_fieldNPC.use_count() > 0)
        {
            m_fieldNPC->update(player.getPosition());
        }
    }

    if (!waveComplete)
    {
        waveSpawning();
    }
}

void Game::gameplayDraw()
{
    if (time == DAYCYCLE::DAY)
    {
        DrawTexturePro(m_ground, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);
    }
    else if (time == DAYCYCLE::NIGHT)
    {
        DrawTexturePro(m_ground, { 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);
    }

    DrawFPS(0, 0);
    if (currentCell == CENTRE)
    {
        for (auto& building : buildings)
        {
            building->draw();
        }

        for (auto& npc : village)
        {
            npc->draw();
        }

        if (m_raid)
        {
            for (auto& npc : barracks)
            {
                npc->draw();
            }
        }
    }
    else
    {
        if (m_fieldNPC.use_count() > 0)
        {
            m_fieldNPC->draw();
        }
    }

    player.draw();
    for (Enemy& enemy : enemies)
    {
        enemy.draw();
    }
    for (auto& npc : party)
    {
        npc->draw();
    }
    for (auto& supply : supplies)
    {
        supply->draw();
    }

    if (currentCell == CENTRE)
    {
        DrawRectangleRec(m_swarmArea, GREEN);
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

    m_dayText = "Day: " + std::to_string(m_dayCount);
    DrawText(m_dayText.c_str(), 10, 70, 20, WHITE);

    DrawText(m_timeText.c_str(), 10, 90, 20, WHITE);

    if (currentCell == CENTRE)
    {
        std::string happinessText = "Town Happiness: " + std::to_string(m_happiness);
        DrawText(happinessText.c_str(), 200, 10, 20, WHITE);
    }

    DrawRectangleRec(m_healthOutline, BLACK);
    DrawRectangleRec(m_healthBar, GREEN);
}

void Game::gameOverDraw()
{
    DrawTexturePro(m_ground, { 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling1.x, m_scrolling1.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);
    DrawTexturePro(m_ground, { 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling2.x, m_scrolling2.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);

    for (auto& button : buttons)
    {
        button->draw();
    }

    std::string titleText = "gameover";
    DrawText(titleText.c_str(), 0, 0, 40, WHITE);
}

void Game::gameOverUpdate()
{
    screenScrolling();

    for (auto& button : buttons)
    {
        if (button->checkCollision(GetMousePosition()) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (button->getTitle() == "Try Again")
            {
                m_gameState = GameState::GAMEPLAY;
                reset();
            }
        }
    }
}

void Game::endOfDayDraw()
{
    DrawTexturePro(m_ground, { 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling1.x, m_scrolling1.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);
    DrawTexturePro(m_ground, { 0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT }, { m_scrolling2.x, m_scrolling2.y, SCREEN_WIDTH, SCREEN_HEIGHT }, { 0,0 }, 0.0f, WHITE);

    for (auto& button : buttons)
    {
        button->draw();
    }

    std::string dailyText;
    dailyText = "Supplies Gained: " + std::to_string(m_dailySuppliesGained) + "\n";
    dailyText += "Supplies Consumed: " + std::to_string(m_dailySuppliesConsumed) + "\n";
    dailyText += "Villagers Added: " + std::to_string(m_dailyVillagers) + "\n";
    dailyText += "Happiness Gained: " + std::to_string(m_dailyHappiness) + "\n";

    DrawText(dailyText.c_str(), 370.0f, 200.0f, 40, WHITE);
}

void Game::endOfDayUpdate()
{
    for (auto& button : buttons)
    {
        if (button->checkCollision(GetMousePosition()) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            if (button->getTitle() == "Done")
            {
                m_gameState = GameState::GAMEPLAY;
                m_dailySuppliesConsumed = 0;
                m_dailySuppliesGained = 0;
                m_dailyVillagers = 0;
                m_dailyHappiness = 0;
            }
        }
    }
}

void Game::screenScrolling()
{
    float speed = 1.5f;

    m_scrolling1.x += speed;
    m_scrolling2.x += speed;

    if (m_scrolling1.x > SCREEN_WIDTH)
    {
        m_scrolling1.x = -SCREEN_WIDTH + 10;
    }

    if (m_scrolling2.x > SCREEN_WIDTH)
    {
        m_scrolling2.x = -SCREEN_WIDTH + 10;
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

            for (auto& npc : party)
            {
                if (npc->isActive())
                {
                    if (CheckCollisionCircles(npc->getBulletPos(), npc->getBulletRadius(), enemy.getPosition(), enemy.getRadius()))
                    {
                        enemy.damage();
                        npc->resetBullet();
                    }

                    if (CheckCollisionCircles(npc->getPosition(), npc->getRadius(), enemy.getPosition(), enemy.getRadius()))
                    {
                        npc->damage();
                        enemy.recoil(20.0f);
                    }
                }
            }
            if (m_raid)
            {
                for (auto& npc : barracks)
                {
                    if (npc->isActive())
                    {
                        if (CheckCollisionCircles(npc->getBulletPos(), npc->getBulletRadius(), enemy.getPosition(), enemy.getRadius()))
                        {
                            enemy.damage();
                            npc->resetBullet();
                        }

                        if (CheckCollisionCircles(npc->getPosition(), npc->getRadius(), enemy.getPosition(), enemy.getRadius()))
                        {
                            npc->damage();
                            enemy.recoil(20.0f);
                        }
                    }
                }

                for (auto& npc : village)
                {
                    if (npc->isActive())
                    {
                        if (CheckCollisionCircles(npc->getPosition(), npc->getRadius(), enemy.getPosition(), enemy.getRadius()))
                        {
                            npc->damage();
                            enemy.recoil(20.0f);
                        }
                    }
                }
            }
        }
    }

    if (m_fieldNPC.use_count() > 0)
    {
        if (CheckCollisionCircles(player.getPosition(), player.getRadius(), m_fieldNPC->getPosition(), m_fieldNPC->getRadius()))
        {
            m_fieldNPC->startFollowing();
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
                m_dailySuppliesGained += 10;
            }
        }
    }

    if (currentCell == CENTRE && IsKeyReleased(KEY_E))
    {
        for (auto& building : buildings)
        {
            canInteract = false;
            if (CheckCollisionCircleRec(player.getPosition(), player.getRadius(), building->getBody()))
            {
                if (!building->completed() && !building->inProcess())
                {
                    building->build(player.currentSupply());
                    player.subtractSupply(building->subractValue());
                }
                else if (building->inProcess())
                {
                    buildingProgress(*building);
                }
                else
                {
                    building->interact(player.currentSupply());
                }
                if (!building->inProcess())
                {
                    player.subtractSupply(building->subractValue());
                }
                buildingInteract(*building);
            }
        }
    }

    if (currentCell == CENTRE && IsKeyReleased(KEY_R))
    {
        for (auto& building : buildings)
        {
            canInteract = false;
            if (CheckCollisionCircleRec(player.getPosition(), player.getRadius(), building->getBody()))
            {
                if (building->inProcess() && village.size() > 0)
                {
                    building->assignBuilder(village.back(), m_dayCount);
                    village.back()->assignToBuilding(building->getPosition());
                }
            }
        }
    }
}

void Game::bunksInteract(Building& t_bunks)
{
    int limit = 0;

    if (t_bunks.completed() && t_bunks.getName() == "Sleeping Bag")
    {
        if (time == DAYCYCLE::NIGHT)
        {
            dayCycle(true);
        }
        return;
    }

    if (t_bunks.completed())
    {
        if (time == DAYCYCLE::NIGHT)
        {
            dayCycle(true);
            return;
        }

        if (party.size() == 0)
        {
            if (village.size() > MAX_PARTY)
            {
                limit = MAX_PARTY;
            }
            else
            {
                limit = village.size();
            }
            for (int i = limit - 1; i >= 0; i--)
            {
                party.push_back(village.back());
                village.pop_back();
                std::cout << party.back()->getName() << " has been added to the party\n";
                m_numInVillage--;
                party.back()->exitPassive();
            }

            return;
        }

        for (int i = party.size() - 1; i >= 0; i--)
        {
            village.push_back(party.at(i));
            party.pop_back();
            std::cout << village.at(m_numInVillage)->getName() << " is in the bunks\n";
            m_numInVillage++;
            village.back()->resetTarget();
        }
    }
}

void Game::buildingInteract(Building& t_building)
{
    std::vector<std::shared_ptr<NPC>> npcs;

    if (t_building.getName() == "Bunks" || t_building.getName() == "Sleeping Bag")
    {
        bunksInteract(t_building);
        return;
    }
    
    if (t_building.completed())
    {
        if (party.size() > 0)
        {
            for (int i = party.size() - 1; i >= 0; i--)
            {
                npcs.push_back(party.back());
                party.pop_back();
                std::cout << npcs.back()->getName() << " was added to the " << t_building.getName() << "\n";
            }
        }
        else
        {
            npcs = barracks;
            for (int i = npcs.size() - 1; i >= 0; i--)
            {
                party.push_back(npcs.back());
                npcs.pop_back();
                std::cout << party.back()->getName() << " was added to the party\n";
            }
        }

        if (t_building.getName() == "Barracks")
        {
            for (auto& npc : npcs)
            {
                barracks.push_back(npcs.back());
                npcs.pop_back();
            }
        }
    }
}

Vector2 Game::findNearestTarget(Enemy& t_enemy, std::vector<std::shared_ptr<NPC>>& t_targets)
{
    float distance = Vector2Distance(player.getPosition(), t_enemy.getPosition());
    float tempDistance = 100000;
    Vector2 nearest = player.getPosition();

    for (auto& npc : t_targets)
    {
        if (npc->isActive())
        {
            tempDistance = Vector2Distance(npc->getPosition(), t_enemy.getPosition());

            if (tempDistance < distance)
            {
                distance = tempDistance;
                nearest = npc->getPosition();
            }
        }
    }

    return nearest;
}

void Game::enemySpawning(int t_num)
{
    if (currentCell == swarmSpot || m_raid)
    {
        if ((currentCell != CENTRE && time == DAYCYCLE::DAY) || (m_raid && currentCell == CENTRE))
        {
            Vector2 start = { 0.0f, 0.0f };
            Direction direction;

            for (int i = 0; i < t_num; i++)
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

                enemies[i].spawn(start);
                waveComplete = false;
            }
        }
    }
}

void Game::waveSpawning()
{
    enemyKill = 0;

    for (auto enemy : enemies)
    {
        if (!enemy.isActive() && (currentCell != CENTRE || m_raid))
        {
            enemyKill++;
        }
    }

    if (enemyKill == MAX_ENEMIES)
    {
        if (currentCell != CENTRE)
        {
            supplySpawning(1);
        }
        waveComplete = true;
        raidStarting(false);
        m_raid = false;
        swarmSpawning();
    }
}

void Game::swarmSpawning()
{
    swarmSpot = (Cell)(rand() % 4);

    switch (swarmSpot)
    {
    case CELLSOUTH:
        m_swarmArea.x = 400.0f;
        m_swarmArea.y = SCREEN_HEIGHT - 50.0f;
        break;
    case CELLNORTH:
        m_swarmArea.x = 400.0f;
        m_swarmArea.y = 50.0f;
        break;
    case CELLWEST:
        m_swarmArea.x = 50.0f;
        m_swarmArea.y = 300.0f;
        break;
    case CELLEAST:
        m_swarmArea.x = SCREEN_WIDTH - 50.0f;
        m_swarmArea.y = 300.0f;
        break;
    default:
        break;
    }
}

void Game::supplySpawning(int t_amount)
{
    if (currentCell == swarmSpot)
    {
        Vector2 pos;

        for (int i = 0; i < t_amount; i++)
        {
            if (!supplies.at(i)->isActive())
            {
                pos.x = (rand() % SCREEN_WIDTH - 50) + 50;
                pos.y = (rand() % SCREEN_HEIGHT - 50) + 50;

                supplies.at(i)->spawn(pos);
            }
        }
    }
    else
    {
        supplies.at(0)->kill();
    }
}

void Game::fieldNPCSpawning()
{
    if (currentCell == swarmSpot)
    {
        int randNum = rand() % 4;
        randNum = 0;

        if (randNum == 0)
        {
            m_fieldNPC = std::make_shared<NPC>(NPC("Barry", { 500.0f, 200.0f }, std::make_shared<OffensiveBehaviour>(), 8));
        }
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
        }
        else if (player.getPosition().x >= SCREEN_WIDTH)
        {
            currentCell = CELLEAST;
            player.setPositionX(0.0f);
            enemySpawning(MAX_ENEMIES);
        }
        else if (player.getPosition().y <= 0.0f)
        {
            currentCell = CELLNORTH;
            player.setPositionY(SCREEN_HEIGHT);
            enemySpawning(MAX_ENEMIES);
        }
        else if (player.getPosition().y >= SCREEN_HEIGHT)
        {
            currentCell = CELLSOUTH;
            player.setPositionY(0.0f);
            enemySpawning(MAX_ENEMIES);
        }

        float offset = 50.0f;

        for (auto& npc : party)
        {
            if (npc->isActive())
            {
                if (currentCell <= CELLSOUTH)
                {
                    npc->newArea(currentCell, { player.getPosition().x + offset, player.getPosition().y });
                    offset += offset;
                }
                if (currentCell > CELLSOUTH && currentCell < CENTRE)
                {
                    npc->newArea(currentCell, { player.getPosition().x, player.getPosition().y + offset });
                    offset += offset;
                }
            }
        }

        if (currentCell != CENTRE)
        {
            fieldNPCSpawning();
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

    if (currentCell == CENTRE)
    {
        float x;
        float y;

        for (auto& npc : party)
        {
            x = (rand() % 400) + 200;
            y = (rand() % 400) + 200;
            npc->newArea(currentCell, { x, y });
        }

        if (m_fieldNPC.use_count() > 0)
        {
            if (m_fieldNPC->isFollowing())
            {
                if (party.size() < MAX_PARTY)
                {
                    m_fieldNPC->stopFollowing();
                    party.push_back(m_fieldNPC);
                    m_fieldNPC.reset();
                    m_dailyVillagers++;
                }
                else if (village.size() < MAX_NPCS)
                {
                    m_fieldNPC->stopFollowing();
                    village.push_back(m_fieldNPC);
                    m_fieldNPC.reset();
                    m_numInVillage++;
                    village.back()->resetTarget();
                    m_dailyVillagers++;
                }
                else
                {
                    m_fieldNPC->kill();
                }
            }
        }
    }

}

void Game::dayCycle(bool t_forcedChange) // If t_forcedChange -> true, force it to day, else function as normal
{
    float timeDivision;

    if (time == DAYCYCLE::DAY)
    {
        if (timedt == 0)
        {
            checkBuildingStatus();
        }

        timedt += GetFrameTime();
        timeDivision = timedt / DAY_LENGTH;

        if (timeDivision < 0.25)
        {
            m_timeText = "Early Morning";
        }
        else if (timeDivision >= 0.25 && timeDivision < 0.4)
        {
            m_timeText = "Morning";
        }
        else if (timeDivision >= 0.4 && timeDivision < 0.7)
        {
            m_timeText = "High Noon";
        }
        else if (timeDivision >= 0.7 && timeDivision < 1)
        {
            m_timeText = "Afternoon";
        }
        else if (timeDivision >= 1)
        {
            m_timeText = "Sunset";
        }
    }

    if (timedt >= DAY_LENGTH && time == DAYCYCLE::DAY && !t_forcedChange && currentCell == CENTRE)
    {
        time = DAYCYCLE::NIGHT;
        timedt = 0.0f;
        m_timeText = "Night";

        if (m_dayCount % RAID_DAY == 0)
        {
            m_raid = true;
            enemySpawning(MAX_ENEMIES);
            raidStarting(true);
        }

        return;
    }

    if (t_forcedChange && time == DAYCYCLE::NIGHT && !m_raid)
    {
        dailySupplies();
        for (auto& building : buildings)
        {
            if (building->getName() == "Saloon")
            {
                building->dailyCheck();
                m_happiness += building->bonusHappiness();
                m_dailyHappiness += building->bonusHappiness();
            }
        }
        m_gameState = GameState::ENDOFDAY;
        time = DAYCYCLE::DAY;
        timedt = 0.0f;
        m_dayCount++;
    }
}

void Game::dailySupplies()
{
    if (m_dayCount > 0)
    {
        int smallSupplies = 0;
        int suppliesUsed = 10;
        suppliesUsed += 10 * party.size();
        suppliesUsed += 10 * village.size();
        suppliesUsed += 10 * barracks.size();

        if (!buildings.at(0)->completed())
        {
            if (player.getSupplies() >= suppliesUsed)
            {
                player.subtractSupply(suppliesUsed);
                if (m_happiness < 50)
                {
                    m_happiness += 10;
                    m_dailyHappiness += 10;
                }
            }
            else if (player.getSupplies() == 0)
            {
                m_happiness -= 10;
                m_dailyHappiness -= 10;
            }
            else
            {
                smallSupplies = suppliesUsed - player.getSupplies();
                player.subtractSupply(smallSupplies);
                suppliesUsed -= smallSupplies;
                m_happiness -= suppliesUsed / 2;
                m_dailyHappiness -= suppliesUsed / 2;
            }
        }
        else
        {
            if (buildings.at(0)->getValue() > suppliesUsed)
            {
                buildings.at(0)->interact(-suppliesUsed);
                if (m_happiness < 50)
                {
                    m_happiness += 10;
                    m_dailyHappiness += 10;
                }
            }
            else
            {
                smallSupplies = suppliesUsed - buildings.at(0)->getValue();
                buildings.at(0)->interact(-smallSupplies);
                suppliesUsed -= smallSupplies;
                m_happiness -= suppliesUsed / 2;
                m_dailyHappiness -= suppliesUsed / 2;
            }
        }

        m_dailySuppliesConsumed = suppliesUsed;
    }
}

void Game::raidStarting(bool t_trigger)
{
    if (t_trigger)
    {
        for (auto& npc : village)
        {
            npc->exitPassive();
        }
    }
    else
    {
        for (auto& npc : village)
        {
            npc->passive();
        }
    }
}

void Game::checkBuildingStatus()
{
    for (auto& building : buildings)
    {
        if (!building->completed())
        {
            if (building->inProcess())
            {
                if (building->hasBuilder())
                {
                    building->applyProgress();
                }
                building->completeCheck(m_dayCount);
            }
        }
    }
}

void Game::buildingProgress(Building& t_building)
{
    if (time == DAYCYCLE::DAY)
    {
        timedt = DAY_LENGTH;
        t_building.applyProgress();
    }
    else
    {
        timedt = 0.0f;
    }
}

void Game::disableSleepingBag()
{
    bool bunksOn = false;
    bool bagOn = false;

    for (int i = 0; i < m_effectiveMaxBuildings; i++)
    {
        if (buildings.at(i)->getName() == "Bunks" && buildings.at(i)->completed())
        {
            bunksOn = true;
        }
        if (buildings.at(i)->getName() == "Sleeping Bag")
        {
            bagOn = true;
        }
    }

    if (bunksOn && bagOn)
    {
        buildings.pop_back();
        m_effectiveMaxBuildings--;
    }
}

