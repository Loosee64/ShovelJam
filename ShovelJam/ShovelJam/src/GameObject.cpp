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
