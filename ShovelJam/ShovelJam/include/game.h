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

private:
    Player player;
    Enemy enemy;
};

#endif // GAME_H
