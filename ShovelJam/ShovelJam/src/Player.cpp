#include "Player.h"

Player::Player() : activeBullet(-1), iFrames(0)
{
	init();
}

void Player::init()
{
	m_position = { 50.0f, 100.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = YELLOW;
	m_radius = 25.0f;
	m_speed = 5.0f;
	m_health = 5;
}

void Player::update()
{
	if (iFrames > 0)
	{
		iFrames--;
	}

	if (m_health <= 0)
	{
		kill();
	}

	input();
	movement();

	for (Bullet& bullet : bullets)
	{
		bullet.update();
	}
}


void Player::movement()
{
	m_position += m_velocity;
	m_velocity = { 0.0f, 0.0f };
}

void Player::kill()
{
	m_colour = BLUE;
}

void Player::damage()
{
	if (iFrames <= 0)
	{
		m_health--;
		//iFrames = MAX_IFRAMES;
	}
	std::cout << m_health << "\n";
}

void Player::draw()
{
	GameObject::draw();
	for (Bullet& bullet : bullets)
	{
		bullet.draw();
	}
}

void Player::input()
{
	if (IsKeyDown(KEY_W)) { m_velocity.y = -m_speed; }
	if (IsKeyDown(KEY_S)) { m_velocity.y = m_speed; }
	if (IsKeyDown(KEY_A)) { m_velocity.x = -m_speed; }
	if (IsKeyDown(KEY_D)) { m_velocity.x = m_speed; }
	
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { shoot(); }
}

void Player::shoot()
{
	Vector2 mousePos = GetMousePosition();

	if (activeBullet < MAX_BULLETS - 1)
	{
		activeBullet++;
	}
	else
	{
		activeBullet = 0;
	}
	bullets[activeBullet].fire(m_position, mousePos);
}

void Player::resetBullet()
{
	if (activeBullet > -1)
	{
		bullets[activeBullet].reset();
	}
}
