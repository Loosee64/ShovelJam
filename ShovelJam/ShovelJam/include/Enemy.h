#pragma once
#include "GameObject.h"
#include "random"

class Enemy :
    public GameObject
{
public:
    Enemy();
    virtual void init() override;
    virtual void movement() override;
    virtual void damage() override;

    void update();
    void draw();

    int getHealth() { return m_health; }

    void spawn(Vector2 t_start);
    void recoil(float t_scale);
    void assignTarget(Vector2 t_target);

private:
    Direction m_direction;
    Vector2 m_target;
};

