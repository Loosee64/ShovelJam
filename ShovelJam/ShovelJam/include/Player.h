#pragma once
#include "GameObject.h"

class Player :
    public GameObject
{
public:
    Player();
    virtual void update();

    void movement();
    void input();

private:
    float m_speed;
};

