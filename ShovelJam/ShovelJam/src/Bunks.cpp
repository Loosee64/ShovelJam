#include "Bunks.h"

void Bunks::assignValues()
{
	m_time = 1;
	m_value = 10;
	m_name = "Bunks";
}

void Bunks::processSupply(Building& t_building, int t_supply)
{
	t_building.m_subtract = 0;
}

void Bunks::interact(Building& t_building)
{
}
