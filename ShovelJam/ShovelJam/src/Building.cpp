#include "Building.h"

Building::Building(std::string t_type, int t_time, int t_value) : m_type(t_type), m_time(t_time), m_value(t_value), m_currentValue(0), m_remainder(0), displayText(""),
																	m_height(0), m_width(0), m_isComplete(false)
{
}

void Building::init()
{
	m_colour = DARKBLUE;
	m_height = 70.0f;
	m_width = 70.0f;

	m_active = true; // -------------------- TEMP
}

void Building::update()
{
	if (m_isComplete)
	{
		m_colour = RED;
	}
}

void Building::draw()
{
	DrawRectangleRec(m_body, m_colour);
	displayText = m_type + "\n" + std::to_string(m_currentValue) + "\n" + std::to_string(m_value);
	DrawText(displayText.c_str(), m_position.x, m_position.y - 70, 20, m_colour);
}

void Building::spawn(Vector2 t_pos)
{
	m_position = t_pos;
	m_body.x = m_position.x;
	m_body.y = m_position.y;
	m_body.width = m_width;
	m_body.height = m_height;
}

void Building::build(int t_valueApplied)
{
	if (m_currentValue < m_value && t_valueApplied > 0)
	{
		m_currentValue += t_valueApplied;
		if (m_currentValue > m_value)
		{
			m_remainder = m_currentValue - m_value;
			m_currentValue -= m_remainder;
		}
		if (m_currentValue == m_value)
		{
			m_isComplete = true;
		}
	}
}
