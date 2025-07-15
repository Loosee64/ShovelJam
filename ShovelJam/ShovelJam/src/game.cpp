#include "raylib.h"
#include "stdio.h"
#include "../include/game.h"

Game::Game() : m_numTargets(0), currentCell(CENTRE), dt(0), wavedt(0), enemyKill(0), waveComplete(false), m_numInVillage(0), canInteract(true), time(DAYCYCLE::DAY),
                timedt(0.0f), m_dayCount(1), m_dayText(""), m_raid(false), m_timeText("")
{
}

void Game::init()
{
    enemySpawning(MAX_ENEMIES);

    village.reserve(MAX_NPCS);

    party.reserve(MAX_PARTY);
    party.push_back(std::make_shared<NPC>(NPC("Barry", { 500.0f, 200.0f }, std::make_shared<OffensiveBehaviour>())));

    supplies.reserve(MAX_SUPPLIES);
    supplies.push_back(std::make_shared<Supply>());

    buildings.reserve(MAX_BUILDINGS);
    buildings.push_back(std::make_shared<Building>(std::make_shared<SupplyShed>()));
    buildings.push_back(std::make_shared<Building>(std::make_shared<Bunks>()));
    buildings.push_back(std::make_shared<Building>(std::make_shared<Barracks>()));
    buildings.push_back(std::make_shared<Building>(std::make_shared<SleepingBag>()));

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

        building->spawn({ 200.0f + offset, 200.0f });
        offset += 300.0f;
    }
}

void Game::draw()
{
    if (time == DAYCYCLE::DAY)
    {
        ClearBackground(ORANGE);
    }
    else if (time == DAYCYCLE::NIGHT)
    {
        ClearBackground(BLACK);
    }

    DrawFPS(0, 0);
    player.draw();
    for (Enemy& enemy : enemies)
    {
        enemy.draw();
    }
    for (auto &npc : party)
    {
        npc->draw();
    }
    for (auto& supply : supplies)
    {
        supply->draw();
    }

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
}

void Game::update()
{
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
        enemy.update(player.getPosition());
    }

    collisionCheck();

    disableSleepingBag();

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

    if (t_bunks.completed() || t_bunks.inProcess())
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

void Game::enemySpawning(int t_num)
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
    }
}

void Game::supplySpawning(int t_amount)
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

void Game::fieldNPCSpawning()
{
    int randNum = rand() % 4;

    if (randNum == 0)
    {
        m_fieldNPC = std::make_shared<NPC>(NPC("Barry", { 500.0f, 200.0f }, std::make_shared<OffensiveBehaviour>()));
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
                }
                else if (village.size() < MAX_NPCS)
                {
                    m_fieldNPC->stopFollowing();
                    village.push_back(m_fieldNPC);
                    m_fieldNPC.reset();
                    m_numInVillage++;
                    village.back()->resetTarget();
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
        time = DAYCYCLE::DAY;
        timedt = 0.0f;
        m_dayCount++;
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

