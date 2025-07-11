#include "Player.h"

Player::Player() : m_speed(5.0f)
{
}

void Player::update()
{
	input();
	movement();
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
}
