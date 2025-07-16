#include "Building.h"

Building::Building(std::shared_ptr<BuildingType> t_type, int t_sprite) : m_sprite(t_sprite), m_currentValue(0), m_remainder(0), displayText(""), m_height(0), m_width(0), m_isComplete(false),
															m_type(t_type), m_subtract(0), m_inProcess(false), m_timeStarted(0), m_timeLeft(m_time), m_textDisplay(true)
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

	m_texture = LoadTexture("ASSETS/Spritesheets/buildings_spritesheet.png");
	m_frameRec = { 0, 0, 128, 128 };
	m_frameRec.y = m_sprite * 128;
	m_destRec = { 0, 0, 128, 128 };
	m_animationdt = 0;
	m_baseRow = 0;
	
	m_timeLeft = m_time;
	m_body = Rectangle();

	m_active = true; // -------------------- TEMP
	m_devVisuals = true;
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
	DrawTexturePro(m_texture, m_frameRec, { m_position.x, m_position.y, 192, 192 }, { 64, 128 }, 0.0f, WHITE);
	DrawRectangleRec(m_body, m_colour);
	displayText = m_name + "\n";
	if (m_textDisplay)
	{
		displayText += "Current Supplies: " + std::to_string(m_currentValue) + "\n" + "Needed Supplies: " + std::to_string(m_value) + "\n" + "Days to build: " + std::to_string(m_timeLeft);
		DrawText(displayText.c_str(), m_position.x - 30, m_position.y - 90, 20, m_colour);
	}
	else
	{
		DrawText(displayText.c_str(), m_position.x - 30, m_position.y - 30, 20, m_colour);
	}
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
	if (!m_inProcess)
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
				m_inProcess = true;
				m_colour = BROWN;
			}
		}
	}
}

void Building::removeBuilder()
{
	m_builder->removeFromBuilding();
	m_builder.reset();
}

bool Building::hasBuilder()
{
	if (m_builder != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Building::applyProgress()
{
	m_timeLeft--;
}

void Building::completeCheck(int t_time)
{
	if (m_timeStarted == 0)
	{
		m_timeStarted = t_time;
		m_timeLeft = m_time;
		return;
	}

	if (m_timeLeft <= 0)
	{
		m_timeStarted = 0;
		m_inProcess = false;
		m_isComplete = true;
		if (m_builder != nullptr)
		{
			removeBuilder();
		}
	}
}

void Building::interact(int t_value)
{
	m_type->processSupply(*this, t_value);
	m_type->interact(*this);
}
