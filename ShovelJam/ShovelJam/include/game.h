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

class Game
{
public:
    Game();
    void init();
    void draw();
    void update();

    void collisionCheck();

    void bunksInteract(Building& t_bunks);

    void enemySpawning(int t_num);
    void waveSpawning();
    void supplySpawning(int t_amount);
    void fieldNPCSpawning();

    void findNPCTarget();
    void moveCell();

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
    std::shared_ptr<NPC> m_fieldNPC;
    int m_numInVillage;

    std::vector<std::shared_ptr<Supply>> supplies;
    std::vector<std::shared_ptr<Building>> buildings;

    Vector2 m_targetsArray[MAX_ENEMIES];
    int m_numTargets;

    int map[5]; ///    1
                ///  2 3 4
                ///    5
    Cell currentCell;

    float dt;
};

#endif // GAME_H
