#pragma once
#include "GameObject.h"

class Bullet :
    public GameObject
{
public:
    Bullet();
    virtual void init();
    virtual void update();
    virtual void movement();
    void draw();

    void fire(Vector2 t_start, Vector2 t_target);
    void reset();

private:
    bool m_active;
};

