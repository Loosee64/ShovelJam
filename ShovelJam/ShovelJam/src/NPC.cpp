#include "NPC.h"

NPC::NPC(std::string t_name, Vector2 t_pos, std::shared_ptr<NPCBehaviour> t_behaviour, int t_animation) : m_name(t_name), m_following(false), m_activeBullet(-1), dt(0), m_shootingCooldown(1.0f), m_maxHealth(3.0f),
												m_target({100000.0f, 100000.0f }), approachDistance(100.0f), behaviour(t_behaviour), areaTimer(-1.0f), areaTimerMax(0.5f),
												m_building(false), iFrames(0)
{
	m_position = t_pos;
	m_baseRow = t_animation;
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
	m_speed = MAX_SPEED;
	m_texture = LoadTexture("ASSETS/Spritesheets/jimmyandnpcs.png");
	m_frameRec = { 0, 0, 64, 64 };
	m_destRec = { 0, 0, 128, 128 };
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

	m_animationdt += GetFrameTime();

	if (m_building)
	{
		follow(m_target);
		movement();
		if (m_position.x <= m_target.x + 50 && m_position.x >= m_target.x - 50 &&
			m_position.y <= m_target.y + 50 && m_position.y >= m_target.y - 50)
		{
			m_active = false;
		}
		return;
	}

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

	if (m_animationdt > 0.1f)
	{
		animate();
		m_animationdt = 0.0f;
	}
	movement();

	if (dt > m_shootingCooldown && m_target.x != 100000.0f)
	{
		std::cout << m_target.x << " " << m_target.y << "\n";
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

	retreat();
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

void NPC::damage()
{
	if (iFrames >= MAX_IFRAMES)
	{
		m_health--;
		iFrames = 0;
	}
	std::cout << m_health << "\n";
}

void NPC::retreat()
{
	std::shared_ptr<NPCBehaviour> temp = behaviour;

	if (behaviour != nullptr)
	{
		if (behaviour->behaviourType() == "Offensive")
		{
			if (m_health < 1)
			{
				behaviour = std::make_shared<DefensiveBehaviour>();
			}
			else
			{
				behaviour.reset();
				behaviour = temp;
			}
		}
	}

	temp.reset();
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
		if (t_maxTargets > 0)
		{
			m_target = nearest;
		}
		else
		{
			m_target.x = 100000.0f;
		}
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

void NPC::passive()
{
	if (m_position.x <= m_target.x + 50 && m_position.x >= m_target.x - 50 && 
		m_position.y <= m_target.y + 50 && m_position.y >= m_target.y - 50)
	{
		m_target = behaviour->passive(*this);
	}
	follow(m_target);
	movement();
}

void NPC::exitPassive()
{
	behaviour->exitPassive(*this);
}

void NPC::assignToBuilding(Vector2 t_pos)
{
	m_building = true;
	m_target = t_pos;
	m_speed = MAX_SPEED;
}

void NPC::removeFromBuilding()
{
	m_active = true;
	m_building = false;
	m_target = behaviour->passive(*this);
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

