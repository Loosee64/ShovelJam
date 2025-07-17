#pragma once
#include <string>

class Building;

class BuildingType
{
	friend class Building;
public:
	virtual void assignValues() = 0;
	virtual void processSupply(Building& t_building, int t_supply) = 0;
	virtual void interact(Building& t_building) = 0;
	virtual void dailyCheck(Building& t_building) {};

	virtual int getTime() { return m_time; }
	virtual int getValue() { return m_value; }
	virtual std::string getName() { return m_name; }
protected:
	int m_time;
	int m_value;
	std::string m_name;
};

#include "Building.h"

