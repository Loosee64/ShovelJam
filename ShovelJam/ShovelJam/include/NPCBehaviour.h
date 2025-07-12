#pragma once
#include "raylib.h"
#include "raymath.h"

class NPCBehaviour
{
public:
	virtual Vector2 approach(Vector2 t_position, Vector2 t_target) = 0;
	virtual bool canApproach() = 0;
	virtual void passive() = 0;
};


