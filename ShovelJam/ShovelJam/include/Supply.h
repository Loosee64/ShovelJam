#pragma once
#include "GameObject.h"

class Supply :
    public GameObject
{
public:
    Supply();
    virtual void init() override;

    void draw();
    void spawn(Vector2 t_pos);
    void kill();

    void increaseValue(int t_days);
    int supplyValue() { return m_value; }

private:
    int m_value;
};

