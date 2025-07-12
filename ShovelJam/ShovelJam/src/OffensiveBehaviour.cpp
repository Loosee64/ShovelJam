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

void OffensiveBehaviour::passive()
{
}
