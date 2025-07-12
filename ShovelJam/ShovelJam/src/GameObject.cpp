#include "GameObject.h"

void GameObject::draw()
{
	DrawCircleV(m_position, m_radius, m_colour);
}

void GameObject::update()
{
}

void GameObject::movement()
{
}

void GameObject::damage()
{
	m_health--;
}

void GameObject::kill()
{
}

void GameObject::follow(Vector2& t_target)
{
	Vector2 heading;

	heading = t_target - m_position;
	heading = Vector2Normalize(heading);
	heading = Vector2Scale(heading, m_speed);

	m_velocity = heading;
}
