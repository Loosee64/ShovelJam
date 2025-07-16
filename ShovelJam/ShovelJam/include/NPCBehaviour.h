#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>

class NPC;

class NPCBehaviour
{
public:
	virtual Vector2 approach(Vector2 t_position, Vector2 t_target) = 0;
	virtual bool canApproach() = 0;
	virtual Vector2 passive(NPC &t_npc) = 0;
	virtual void exitPassive(NPC& t_npc) = 0;
	virtual std::string behaviourType() { return "NULL"; };
};

#include "NPC.h"


