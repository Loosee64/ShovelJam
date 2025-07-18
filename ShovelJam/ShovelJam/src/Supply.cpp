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
	m_devVisuals = false;

	m_texture = LoadTexture("ASSETS/Spritesheets/supply.png");
	m_frameRec = { 0, 0, 64, 64 };
	m_destRec = { 0, 0, 128, 128 };
	m_currentFrame = 0;
	m_animationdt = 0;
	m_baseRow = 0;
	m_spriteSize = 64;
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

void Supply::increaseValue(int t_days)
{
	int tempValue = 0;

	if (t_days > 3)
	{
		tempValue = t_days / 2.0f;
		if (tempValue > 1)
		{
			m_value *= tempValue;
		}
	}
}
