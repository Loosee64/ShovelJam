#include "Bullet.h"

Bullet::Bullet() : m_active(false)
{
	init();
}

void Bullet::init()
{
	m_position = { 200.0f, 100.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = WHITE;
	m_radius = 10.0f;
	m_speed = 20.0f;
}

void Bullet::update()
{
	movement();
}

void Bullet::movement()
{
	if (m_active)
	{
		m_position += m_velocity;
	}
}

void Bullet::draw()
{
	if (m_active)
	{
		GameObject::draw();
	}
}

void Bullet::fire(Vector2 t_start, Vector2 t_target)
{
	m_position = t_start;
	
	Vector2 heading = t_target - m_position;
	heading = Vector2Normalize(heading);
	heading = Vector2Scale(heading, m_speed);

	m_velocity = heading;

	m_active = true;
}

void Bullet::reset()
{
	m_position.x = -10000;
	m_velocity = { 0.0f, 0.0f };
	m_active = false;
}
