#include "Building.h"

Building::Building(std::shared_ptr<BuildingType> t_type) : m_currentValue(0), m_remainder(0), displayText(""), m_height(0), m_width(0), m_isComplete(false),
															m_type(t_type), m_subtract(0)
{
}

void Building::init()
{
	m_colour = DARKBLUE;
	m_height = 70.0f;
	m_width = 70.0f;

	m_type->assignValues();

	m_time = m_type->getTime();
	m_value = m_type->getValue();
	m_name = m_type->getName();
	m_body = Rectangle();

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
	displayText = m_name + "\n" + std::to_string(m_currentValue) + "\n" + std::to_string(m_value);
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
	m_remainder = 0;
	m_subtract = 0;
	if (m_currentValue < m_value && t_valueApplied > 0)
	{
		m_currentValue += t_valueApplied;
		if (m_currentValue > m_value)
		{
			m_remainder = m_currentValue - m_value;
			m_subtract = m_currentValue - m_remainder;
			m_currentValue -= m_remainder;
		}
		if (m_currentValue == m_value)
		{
			m_isComplete = true;
		}
	}
}

void Building::interact(int t_value)
{
	m_type->processSupply(*this, t_value);
	m_type->interact(*this);
}
