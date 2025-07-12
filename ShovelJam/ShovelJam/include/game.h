#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include <vector>

class Game
{
public:
    void init();
    void draw();
    void update();

    void collisionCheck();
    void enemySpawning(int t_num);

private:
    static const int MAX_ENEMIES = 10;
    static const int MAX_NPCS = 1;

    Player player;
    Enemy enemies[MAX_ENEMIES];
    std::vector<std::shared_ptr<NPC>> npcs;
};

#endif // GAME_H
