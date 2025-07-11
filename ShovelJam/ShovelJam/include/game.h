#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"

class Game
{
public:
    void init();
    void draw();
    void update();

    void collisionCheck();
    void enemySpawning();

private:
    static const int MAX_ENEMIES = 10;

    Player player;
    Enemy enemy[MAX_ENEMIES];
};

#endif // GAME_H
