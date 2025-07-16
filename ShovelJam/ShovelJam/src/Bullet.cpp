#include "Bullet.h"

Bullet::Bullet()
{
}

void Bullet::init()
{
	m_position = { 100000.0f, 100.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = WHITE;
	m_radius = 10.0f;
	m_speed = 20.0f;
	m_active = false;
	m_devVisuals = true;

	m_texture = LoadTexture("ASSETS/Spritesheets/bullet.png");
	m_frameRec = { 0, 0, 64, 64 };
	m_destRec = { 0, 0, 1, 32 };
	m_currentFrame = 0;
	m_animationdt = 0;
	m_baseRow = 0;
	m_spriteSize = 32;
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
		DrawTexturePro(m_texture, m_frameRec, { m_position.x, m_position.y, 20, 20 }, { 32, 32 }, 0.0f, WHITE);
		//GameObject::draw();
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
