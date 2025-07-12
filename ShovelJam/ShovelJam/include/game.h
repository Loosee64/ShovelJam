#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include "Supply.h"
#include <vector>
#include <string>

enum Cell {CELLNORTH, CELLSOUTH, CELLEAST, CELLWEST, CENTRE};

class Game
{
public:
    Game();
    void init();
    void draw();
    void update();

    void collisionCheck();
    void enemySpawning(int t_num);
    void waveSpawning();
    void supplySpawning(int t_amount);
    void findNPCTarget();
    void moveCell();

private:
    static const int MAX_ENEMIES = 10;
    static const int MAX_NPCS = 3;
    static const int MAX_SUPPLIES = 1;

    Player player;
    Enemy enemies[MAX_ENEMIES];
    int enemyKill;
    float wavedt;
    bool waveComplete;
    std::vector<std::shared_ptr<NPC>> npcs;
    std::vector<std::shared_ptr<Supply>> supplies;

    Vector2 m_targetsArray[MAX_ENEMIES];
    int m_numTargets;

    int map[5]; ///    1
                ///  2 3 4
                ///    5
    Cell currentCell;

    float dt;
};

#endif // GAME_H
