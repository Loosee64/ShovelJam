#include "GameObject.h"

void GameObject::draw()
{
	if (m_devVisuals)
	{
		DrawCircleV(m_position, m_radius, m_colour);
	}
	if (m_texture.id != 0)
	{
		DrawTexturePro(m_texture, m_frameRec, { m_position.x, m_position.y, m_spriteSize, m_spriteSize }, { m_spriteSize / 2, m_spriteSize / 2 }, 0.0f, WHITE);
	}
}

void GameObject::animate()
{
	if (m_currentFrame <= 5)
	{
		m_currentFrame++;
	}
	else
	{
		m_currentFrame = 0;
	}
	m_frameRec.x = m_currentFrame * 64;
	if (m_velocity.x == 0.0f && m_velocity.y == 0.0f)
	{
		swapAnimation(0);
	}
	else if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
	{
		swapAnimation(1);
	}
}

void GameObject::swapAnimation(int t_type)
{
	if (t_type == 0) // Idle
	{
		m_animationRow = m_baseRow;
	}
	else
	{
		m_animationRow = m_baseRow + 1;
	}
	m_frameRec.y = (m_animationRow * 64);
}

void GameObject::update()
{
}

void GameObject::movement()
{
}

void GameObject::damage()
{
	m_health--;
}

void GameObject::kill()
{
	m_active = false;
}

void GameObject::spawn()
{
	m_active = true;
}

void GameObject::follow(Vector2& t_target)
{
	Vector2 heading;

	heading = t_target - m_position;
	heading = Vector2Normalize(heading);
	heading = Vector2Scale(heading, m_speed);

	m_velocity = heading;
}
