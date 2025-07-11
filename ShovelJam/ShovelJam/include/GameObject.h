#pragma once
#include "raylib.h"
#include "raymath.h"

class GameObject
{
public:
	void init();
	virtual void update() = 0;
	void draw();

protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;
	Color m_colour;

};

