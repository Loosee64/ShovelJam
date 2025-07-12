#include "Supply.h"

Supply::Supply() : m_value(10)
{
}

void Supply::init()
{
	m_position = { 100.0f, 500.0f };
	m_colour = BROWN;
	m_radius = 25.0f;
	m_active = false;
}

void Supply::draw()
{
	if (m_active)
	{
		GameObject::draw();
	}
}

void Supply::spawn(Vector2 t_pos)
{
	m_position = t_pos;
	GameObject::spawn();
}

void Supply::kill()
{
	GameObject::kill();
}
