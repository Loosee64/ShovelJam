#pragma once
#include "GameObject.h"
#include "random"

class Enemy :
    public GameObject
{
public:
    Enemy();
    virtual void init();
    virtual void update();
    virtual void movement();

    void draw();

    void spawn(Vector2 t_start);
    void kill();

    bool isActive() { return m_active; }

private:
    Direction m_direction;
    bool m_active;
};

