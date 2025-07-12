#include "Enemy.h"

Enemy::Enemy() : m_active(true)
{
	init();
}

void Enemy::init()
{
	m_position = { 10000.0f, 300.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = RED;
	m_radius = 25.0f;
	m_speed = 2.5f;
}

void Enemy::update()
{
	if (m_active)
	{
		movement();
	}
}


void Enemy::movement()
{
	if (m_position.x < 20.0f) { m_direction = SOUTH; }
	if (m_position.x > 700.0f) { m_direction = NORTH; }

	if (m_direction == NORTH) { m_velocity.x = -m_speed; }
	if (m_direction == SOUTH) { m_velocity.x = m_speed; }

	m_position += m_velocity;
}

void Enemy::draw()
{
	if (m_active)
	{
		GameObject::draw();
	}
}

void Enemy::spawn(Vector2 t_start)
{
	m_position = t_start;
	m_active = true;
}

void Enemy::kill()
{
	m_active = false;
}
