#pragma once
#include "BuildingType.h"
class Saloon :
    public BuildingType
{
public:
    virtual void assignValues() override;
    virtual void processSupply(Building& t_building, int t_supply) override;
    virtual void interact(Building& t_building) override;
    virtual void dailyCheck(Building& t_building) override;
};

