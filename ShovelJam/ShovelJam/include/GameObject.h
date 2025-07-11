#pragma once
#include "raylib.h"
#include "raymath.h"
#include "globals.h"
#include "reasings.h"

class GameObject
{
public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw();
	virtual void movement();

protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;
	Color m_colour;
	float m_speed;
};

