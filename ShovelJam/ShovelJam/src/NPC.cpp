#include "NPC.h"

NPC::NPC(std::string t_name, Vector2 t_pos, std::shared_ptr<NPCBehaviour> t_behaviour) : m_name(t_name), m_following(false), m_activeBullet(-1), dt(0), m_shootingCooldown(1.0f), m_maxHealth(3.0f), 
												m_target({100000.0f, 100000.0f }), approachDistance(100.0f), behaviour(t_behaviour), areaTimer(-1.0f), areaTimerMax(0.5f)
{
	m_position = t_pos;;
	init();
}

NPC::~NPC()
{
}

void NPC::init()
{
	m_velocity = { 0.0f, 0.0f };
	m_colour = SKYBLUE;
	m_radius = 25.0f;
	m_speed = 3.5f;
	m_health = 5;
	
	m_active = true; // ---------- TEMP
}

void NPC::movement()
{
	m_position += m_velocity;
}

void NPC::update(Vector2 t_target)
{
	dt += GetFrameTime();

	if (areaTimer > -1.0f)
	{
		areaTimer += GetFrameTime();
		if (areaTimer >= areaTimerMax)
		{
			areaTimer = -1.0f;
		}
		else
		{
			follow(m_newAreaTarget);
		}
	}

	if (m_following)
	{
		follow(t_target);
	}
	if (areaTimer == -1.0f)
	{
		approachTarget();
	}

	movement();

	if (dt > m_shootingCooldown && m_target.x != 100000.0f)
	{
		shoot(m_target);
		dt = 0;
	}

	if (m_activeBullet > -1)
	{
		for (Bullet& bullet : bullets)
		{
			bullet.update();
		}
	}
}

void NPC::draw()
{
	if (m_active)
	{
		GameObject::draw();
		if (m_following)
		{
			m_colour = GREEN;
		}
		else
		{
			m_colour = SKYBLUE;
		}

		DrawText(m_name.c_str(), m_position.x - 25.0f, m_position.y - 45.0f, 20, SKYBLUE);
		if (m_activeBullet > -1)
		{
			for (Bullet& bullet : bullets)
			{
				bullet.draw();
			}
		}
	}
}

void NPC::spawn(Vector2 t_start)
{
	m_position = t_start;
	m_health = m_maxHealth;
	m_active = true;
}

void NPC::findTarget(Vector2* t_targets, int t_maxTargets)
{
	float nearestValue = 100000;
	Vector2 nearest = { 100000.0f , 100000.0f };
	Vector2 current;
	float currentValue;

	if (m_active)
	{
		for (int i = 0; i < t_maxTargets; i++)
		{
			current = t_targets[i] - m_position;
			currentValue = Vector2Length(current);
			if (currentValue < nearestValue)
			{
				nearestValue = currentValue;
				nearest = t_targets[i];
			}
		}
		m_target = nearest;
	}
}

void NPC::shoot(Vector2 t_target)
{
	if (m_activeBullet < MAX_BULLETS - 1)
	{
		m_activeBullet++;
	}
	else
	{
		m_activeBullet = 0;
	}
	bullets[m_activeBullet].fire(m_position, t_target);
}

void NPC::approachTarget()
{
	if (m_target.x != 100000.0f && !m_following)
	{
		Vector2 trueTarget = behaviour->approach(m_position, m_target);

		if (behaviour->canApproach())
		{
			GameObject::follow(trueTarget);
		}
		else
		{
			m_velocity *= 0.01;
		}
	}
	else if (!m_following)
	{
		m_velocity = { 0.0f, 0.0f };
	}
}

void NPC::newArea(Cell t_direction, Vector2 t_start)
{
	areaTimer = 0.0f;

	switch (t_direction)
	{
	case SOUTH:
		m_position = t_start;
		m_newAreaTarget = t_start;
		m_newAreaTarget.y -= 100.0f;
		break;
	case NORTH:
		m_position = t_start;
		m_newAreaTarget = t_start;
		m_newAreaTarget.y += 100.0f;
		break;
	case WEST:
		m_position = t_start;
		m_newAreaTarget = t_start;
		m_newAreaTarget.x += 100.0f;
		break;
	case EAST:
		m_position = t_start;
		m_newAreaTarget = t_start;
		m_newAreaTarget.x -= 100.0f;
		break;
	case BASE:
		m_position = t_start;
		m_newAreaTarget = t_start;
		areaTimer = areaTimerMax;
		break;
	default:
		break;
	}
}

void NPC::resetBullet()
{
	if (m_activeBullet > -1)
	{
		bullets[m_activeBullet].reset();
	}
}

