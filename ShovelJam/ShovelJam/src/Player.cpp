#include "Player.h"

Player::Player()
{
	init();
}

void Player::init()
{
	m_position = { 100.0f, 100.0f };
	m_velocity = { 0.0f, 0.0f };
	m_colour = YELLOW;
	m_radius = 25.0f;
	m_speed = 5.0f;
}

void Player::update()
{
	input();
	movement();
	bullet.update();
}

void Player::draw()
{
	DrawCircleV(m_position, m_radius, m_colour);
	bullet.draw();
}

void Player::movement()
{
	m_position += m_velocity;
	m_velocity = { 0.0f, 0.0f };
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
	bullet.fire(m_position, mousePos);
}

void Player::resetBullet()
{
	bullet.reset();
}
