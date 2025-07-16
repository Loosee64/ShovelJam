#include "Enemy.h"

Enemy::Enemy() : m_target({1000.0f, 1000.0f})
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

void Enemy::update()
{
	if (m_active)
	{
		if (m_health <= 0)
		{
			kill();
		}

		follow(m_target);
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
	m_target = { 100000.0f, 100000.0f };
	GameObject::spawn();
}

void Enemy::recoil(float t_scale)
{
	m_position -= Vector2Scale(m_velocity, t_scale);
}

void Enemy::assignTarget(Vector2 t_target)
{
	float newDistance = Vector2Distance(t_target, m_position);
	float oldDistance = Vector2Distance(m_target, m_position);

	if (newDistance < oldDistance)
	{
		m_target = t_target;
	}
}
