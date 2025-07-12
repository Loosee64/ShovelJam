#include "Enemy.h"

Enemy::Enemy()
{
	init();
}

void Enemy::init()
{
	m_position = { 10000.0f, 300.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = RED;
	m_radius = 25.0f;
	m_speed = 1.5f;
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

		follow(t_target);
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
	GameObject::spawn();
}

void Enemy::recoil(float t_scale)
{
	m_position -= Vector2Scale(m_velocity, t_scale);
}
