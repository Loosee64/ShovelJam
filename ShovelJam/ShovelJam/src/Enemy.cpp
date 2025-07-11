#include "Enemy.h"

Enemy::Enemy()
{
	init();
}

void Enemy::init()
{
	m_position = { 500.0f, 300.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = RED;
	m_radius = 25.0f;
	m_speed = 2.5f;
}

void Enemy::update()
{
	movement();
}


void Enemy::movement()
{
	if (m_position.y < 20.0f) { m_direction = SOUTH; }
	if (m_position.y > 700.0f) { m_direction = NORTH; }

	if (m_direction == NORTH) { m_velocity.y = -m_speed; }
	if (m_direction == SOUTH) { m_velocity.y = m_speed; }

	m_position += m_velocity;
}

void Enemy::kill()
{
	m_position.x = 10000;
}
