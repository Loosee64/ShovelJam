#include "OffensiveBehaviour.h"

Vector2 OffensiveBehaviour::approach(Vector2 t_position, Vector2 t_target)
{
	if (t_target.x != 100000.0f)
	{
		Vector2 heading = t_target - t_position;
		distance = Vector2Length(heading);

		return t_target;
	}
	return t_position;
}

bool OffensiveBehaviour::canApproach()
{
	if (distance > MAX_DISTANCE)
	{
		return true;
	}
	return false;
}

Vector2 OffensiveBehaviour::passive(NPC& t_npc)
{
	Vector2 randVector;

	randVector.x = (rand() % 500) + 200;
	randVector.y = (rand() % 500) + 200;
	t_npc.m_speed = 0.6f;

	return randVector;
}

void OffensiveBehaviour::exitPassive(NPC& t_npc)
{
	t_npc.m_speed = t_npc.MAX_SPEED;
}
