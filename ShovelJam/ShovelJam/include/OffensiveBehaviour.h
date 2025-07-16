#pragma once
#include "NPCBehaviour.h"
#include <random>

class OffensiveBehaviour :
    public NPCBehaviour
{
public:
    virtual Vector2 approach(Vector2 t_position, Vector2 t_target) override;
    virtual bool canApproach() override;
    virtual Vector2 passive(NPC& t_npc) override;
    virtual void exitPassive(NPC& t_npc) override;
    virtual std::string behaviourType() { return "Offensive"; }
private:
    float distance;
    const float MAX_DISTANCE = 100.0f;
};
