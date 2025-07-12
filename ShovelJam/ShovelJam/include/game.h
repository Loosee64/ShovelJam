#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include <vector>

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
    void findNPCTarget();
    void moveCell();

private:
    static const int MAX_ENEMIES = 10;
    static const int MAX_NPCS = 1;

    Player player;
    Enemy enemies[MAX_ENEMIES];
    std::vector<std::shared_ptr<NPC>> npcs;

    Vector2 m_targetsArray[MAX_ENEMIES];
    int m_numTargets;

    int map[5]; ///    1
                ///  2 3 4
                ///    5
    Cell currentCell;

    float dt;
};

#endif // GAME_H
