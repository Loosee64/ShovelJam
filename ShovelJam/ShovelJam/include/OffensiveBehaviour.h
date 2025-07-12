#pragma once
#include "NPCBehaviour.h"

class OffensiveBehaviour :
    public NPCBehaviour
{
public:
    virtual Vector2 approach(Vector2 t_position, Vector2 t_target) override;
    virtual bool canApproach() override;
    virtual void passive() override;
private:
    float distance;
    const float MAX_DISTANCE = 100.0f;
};
