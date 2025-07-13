#include "Barracks.h"

void Barracks::assignValues()
{
	m_time = 1;
	m_value = 50;
	m_name = "Barracks";
}

void Barracks::processSupply(Building& t_building, int t_supply)
{
	t_building.m_subtract = 0;
}

void Barracks::interact(Building& t_building)
{
}
