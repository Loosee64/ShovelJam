#pragma once
#include <string>
#include "raylib.h"
#include "globals.h"

class Button
{
public:
	Button(std::string t_title, float t_posY);
	void init();

	void draw();
	std::string getTitle() { return m_title; }

	void setValues(std::string t_title, float t_y) { m_title = t_title; m_position.y = t_y; init(); }

	bool checkCollision(Vector2 t_mouse);

private:
	float m_width;
	float m_height;
	float m_outlineOffset;
	float m_textLength;
	int m_fontSize;
	Rectangle m_primary;
	Rectangle m_outline;
	bool m_isHovered;
	std::string m_title;
	Vector2 m_position;
	Vector2 m_textPosition;
};

