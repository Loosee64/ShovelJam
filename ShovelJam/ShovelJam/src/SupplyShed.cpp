#include "SupplyShed.h"

void SupplyShed::assignValues()
{
	m_time = 2;
	m_value = 20;
	m_name = "Supply Shed";
}

void SupplyShed::processSupply(Building& t_building, int t_supply)
{
	t_building.m_currentValue += t_supply;
	t_building.m_subtract = t_supply;
}

void SupplyShed::interact(Building& t_building)
{
}
