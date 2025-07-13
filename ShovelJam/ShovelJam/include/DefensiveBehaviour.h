#pragma once
#include "NPCBehaviour.h"
class DefensiveBehaviour :
    public NPCBehaviour
{
    virtual Vector2 approach(Vector2 t_position, Vector2 t_target) override;
    virtual bool canApproach() override;
    virtual Vector2 passive(NPC& t_npc) override;
    virtual void exitPassive(NPC& t_npc) override;
private:
    float distance;
    const float MAX_DISTANCE = 250.0f;
};

