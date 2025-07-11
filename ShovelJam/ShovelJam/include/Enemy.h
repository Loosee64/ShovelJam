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

    void draw();

    void kill();

    bool isActive() { return m_active; }

private:
    Direction m_direction;
    bool m_active;
};

