#include "DefensiveBehaviour.h"

Vector2 DefensiveBehaviour::approach(Vector2 t_position, Vector2 t_target)
{
	if (t_target.x != 100000.0f)
	{
		Vector2 heading = t_target - t_position;
		distance = Vector2Length(heading);

		heading = t_position - heading;

		heading = Vector2Scale(heading, 2.0f);

		return heading;
	}
	return t_position;
}

bool DefensiveBehaviour::canApproach()
{
	if (distance < MAX_DISTANCE)
	{
		return true;
	}
	return false;
}

Vector2 DefensiveBehaviour::passive(NPC& t_npc)
{
	return t_npc.m_position;
}

void DefensiveBehaviour::exitPassive(NPC& t_npc)
{
}
