#pragma once
#include "GameObject.h"
class Enemy :
    public GameObject
{
public:
    Enemy();
    virtual void init();
    virtual void update();
    virtual void movement();

    void kill();

private:
    Direction m_direction;
};

