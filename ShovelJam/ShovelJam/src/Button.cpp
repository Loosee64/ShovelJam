#include "Button.h"

Button::Button(std::string t_title,float t_posY) : m_title(t_title), m_width(250.0f), m_height(75.0f), m_isHovered(false), m_outlineOffset(10.0f),
														m_textLength(0.0f), m_fontSize(50)
{
	m_position.x = (SCREEN_WIDTH / 2.0f) - (m_width / 2.0f);
	m_position.y = t_posY;
	init();
}

void Button::init()
{
	m_primary = { m_position.x, m_position.y, m_width, m_height };
	m_outline = { m_position.x - (m_outlineOffset / 2), m_position.y - (m_outlineOffset / 2), m_width + m_outlineOffset, m_height + m_outlineOffset };
	m_textLength = MeasureText(m_title.c_str(), m_fontSize);
}

void Button::draw()
{
	if (m_isHovered)
	{
		DrawRectangleRec(m_outline, BLACK);
	}
	DrawRectangleRec(m_primary, WHITE);
	m_textPosition.x = (m_position.x + (m_width / 2.0f)) - (m_textLength / 2.0f);
	m_textPosition.y = m_position.y + (m_fontSize / 4.0f);
	DrawText(m_title.c_str(), m_textPosition.x, m_textPosition.y, m_fontSize, BLACK);
}

bool Button::checkCollision(Vector2 t_mouse)
{
	if (CheckCollisionPointRec(t_mouse, m_primary))
	{
		m_isHovered = true;
		return true;
	}
	m_isHovered = false;
	return false;
}
