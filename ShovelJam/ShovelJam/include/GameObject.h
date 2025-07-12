#pragma once
#include "raylib.h"
#include "raymath.h"
#include "globals.h"
#include "reasings.h"
#include <iostream>

class GameObject
{
public:
	virtual void init() = 0;
	virtual void update();
	virtual void movement();
	virtual void damage();
	virtual void follow(Vector2& t_target);
	virtual bool isActive() { return m_active; }

	void draw();
	void kill();
	void spawn();

	Vector2 getPosition() { return m_position; }
	float getRadius() { return m_radius; }

protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;
	Color m_colour;
	float m_speed;
	int m_health;
	bool m_active;
};

