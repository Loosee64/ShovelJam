#pragma once
#include "GameObject.h"
class NPC :
    public GameObject
{
public:
    NPC();
    virtual void init() override;
    virtual void movement() override;

    void update(Vector2 t_target);
    void draw();
    void startFollowing() { following = true; }

private:
    std::string m_name;
    bool following;
};

