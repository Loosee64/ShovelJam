#include "GameObject.h"

void GameObject::draw()
{
	DrawCircleV(m_position, m_radius, m_colour);
}

void GameObject::movement()
{
}
