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

    void update(Vector2 t_target);
    void draw();

    void spawn(Vector2 t_start);
    void kill();
    void recoil(float t_scale);

    bool isActive() { return m_active; }

private:
    Direction m_direction;
    bool m_active;
};

