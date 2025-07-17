#include "Player.h"

Player::Player() : activeBullet(-1), iFrames(0), dt(0), m_shootingCooldown(0.15f), supplyValue(0)
{
}

void Player::init()
{
	m_position = { 300.0f, 500.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = YELLOW;
	m_radius = 25.0f;
	m_speed = 5.0f;
	m_health = MAX_HEALTH;
	m_texture = LoadTexture("ASSETS/Spritesheets/jimmyandnpcs.png");
	m_frameRec = { 0, 0, 64, 64 };
	m_destRec = { 0, 0, 128, 128 };
	m_currentFrame = 0;
	m_animationdt = 0;
	m_baseRow = 0;
	m_devVisuals = false;
	m_spriteSize = 128;

	for (auto& bullet : bullets)
	{
		bullet.init();
	}
}

void Player::update()
{
	dt += GetFrameTime();

	m_animationdt += GetFrameTime();

	if (iFrames > 0)
	{
		iFrames += GetFrameTime();
	}

	if (m_health <= 0)
	{
		kill();
	}

	input();

	if (m_animationdt >= 0.1)
	{
		animate();
		m_animationdt = 0.0f;
	}

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
	if (iFrames >= MAX_IFRAMES)
	{
		m_health--;
		iFrames = 0;
	}
	std::cout << m_health << "\n";
}

void Player::reset()
{
	m_health = MAX_HEALTH;
	m_currentFrame = 0;
	m_animationdt = 0;
	m_position = { 300.0f, 500.0f };
	supplyValue = 0;
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
	if (IsKeyDown(KEY_W)) { m_velocity.y = -m_speed;}
	if (IsKeyDown(KEY_S)) { m_velocity.y = m_speed;}
	if (IsKeyDown(KEY_A)) { m_velocity.x = -m_speed;}
	if (IsKeyDown(KEY_D)) { m_velocity.x = m_speed;}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && dt >= m_shootingCooldown) { shoot(); dt = 0; }
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

bool Player::isAlive()
{
	if (m_health <= 0)
	{
		return false;
	}
	return true;
}

void Player::resetBullet()
{
	if (activeBullet > -1)
	{
		bullets[activeBullet].reset();
	}
}

void Player::subtractSupply(int t_val)
{
	if (supplyValue >= t_val)
	{
		supplyValue -= t_val;
	}
}
