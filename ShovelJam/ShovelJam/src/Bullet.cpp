#include "Bullet.h"

Bullet::Bullet()
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
	m_position += m_velocity;
}

void Bullet::fire(Vector2 t_start, Vector2 t_target)
{
	m_position = t_start;
	
	Vector2 heading = t_target - m_position;
	heading = Vector2Normalize(heading);
	heading = Vector2Scale(heading, m_speed);

	m_velocity = heading;
}

void Bullet::reset()
{
	m_position.x = -10000;
	m_velocity = { 0.0f, 0.0f };
}
