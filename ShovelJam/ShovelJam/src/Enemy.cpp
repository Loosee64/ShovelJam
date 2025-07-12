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
	m_health = 2;
}

void Enemy::update(Vector2 t_target)
{
	if (m_active)
	{
		if (m_health <= 0)
		{
			kill();
		}

		track(t_target);
		movement();
	}
}

void Enemy::damage()
{
	m_health--;
	recoil(5.0f);
}


void Enemy::movement()
{
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
	m_health = 2;
	m_position = t_start;
	m_active = true;
}

void Enemy::kill()
{
	m_active = false;
}

void Enemy::track(Vector2& t_target)
{
	Vector2 heading;

	heading = t_target - m_position;
	heading = Vector2Normalize(heading);
	heading = Vector2Scale(heading, m_speed);

	m_velocity = heading;
}

void Enemy::recoil(float t_scale)
{
	m_position -= Vector2Scale(m_velocity, t_scale);
}
