#include "NPC.h"

NPC::NPC() : m_name("Fred"), following(false)
{
	init();
}

void NPC::init()
{
	m_position = { 500.0f, 500.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = SKYBLUE;
	m_radius = 25.0f;
	m_speed = 3.5f;
	m_health = 5;
}

void NPC::movement()
{
	m_position += m_velocity;
}

void NPC::update(Vector2 t_target)
{
	if (following)
	{
		follow(t_target);
	}
	movement();
}

void NPC::draw()
{
	GameObject::draw();
	DrawText(m_name.c_str(), m_position.x - 25.0f, m_position.y - 45.0f, 20, SKYBLUE);
}

