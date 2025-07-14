#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include "Supply.h"
#include "Building.h"

#include "Bunks.h"
#include "Barracks.h"
#include "SupplyShed.h"

#include <vector>
#include <string>

enum class DAYCYCLE {DAY, NIGHT};

class Game
{
public:
    Game();
    void init();
    void draw();
    void update();

    void collisionCheck();

    void bunksInteract(Building& t_bunks);
    void buildingInteract(Building& t_building);

    void enemySpawning(int t_num);
    void waveSpawning();
    void supplySpawning(int t_amount);
    void fieldNPCSpawning();

    void findNPCTarget();
    void moveCell();

    void dayCycle(bool t_forcedChange);
    void raidStarting(bool t_trigger);

private:
    static const int MAX_ENEMIES = 10;
    static const int MAX_NPCS = 50;
    static const int MAX_PARTY = 3;
    static const int MAX_SUPPLIES = 1;
    static const int MAX_BUILDINGS = 1;

    Player player;
    Enemy enemies[MAX_ENEMIES];
    int enemyKill;
    float wavedt;
    bool waveComplete;
    bool canInteract;
    std::vector<std::shared_ptr<NPC>> party;
    std::vector<std::shared_ptr<NPC>> village;
    std::vector < std::shared_ptr<NPC>> barracks;
    std::shared_ptr<NPC> m_fieldNPC;
    int m_numInVillage;

    std::vector<std::shared_ptr<Supply>> supplies;
    std::vector<std::shared_ptr<Building>> buildings;

    Vector2 m_targetsArray[MAX_ENEMIES];
    int m_numTargets;

    Cell currentCell;

    float dt;
    float timedt;
    const float DAY_LENGTH = 60.0f;

    DAYCYCLE time;
    int m_dayCount;
    std::string m_dayText;
    std::string m_timeText;
    const int RAID_DAY = 1;

    bool m_raid;
};

#endif // GAME_H
