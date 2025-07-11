#ifndef GAME_H
#define GAME_H

#include "Player.h"

class Game
{
public:
    void init();
    void draw();
    void update();

private:
    Player player;
};

#endif // GAME_H
