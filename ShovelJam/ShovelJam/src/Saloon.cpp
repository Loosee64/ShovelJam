#include "Saloon.h"

void Saloon::assignValues()
{
	m_time = 4;
	m_value = 100;
	m_name = "Saloon";
}

void Saloon::processSupply(Building& t_building, int t_supply)
{
	t_building.m_currentValue += t_supply;
	t_building.m_subtract = t_supply;
}

void Saloon::interact(Building& t_building)
{
}

void Saloon::dailyCheck(Building& t_building)
{
	if (t_building.m_currentValue >= 20 && t_building.m_currentValue < 50)
	{
		t_building.m_bonusHappiness += 10;
		t_building.m_currentValue -= 10;
	}
	else if (t_building.m_currentValue >= 50)
	{
		t_building.m_bonusHappiness += 20;
		t_building.m_currentValue -= 10;
	}
}
