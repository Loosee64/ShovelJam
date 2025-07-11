#include "GameObject.h"

void GameObject::init()
{
	m_position = { 100.0f, 100.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = YELLOW;
	m_radius = 50.0f;
}

void GameObject::draw()
{
	DrawCircleV(m_position, m_radius, m_colour);
}
