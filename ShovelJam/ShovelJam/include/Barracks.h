#pragma once
#include "BuildingType.h"
class Barracks :
    public BuildingType
{
    friend class Building;
    virtual void assignValues() override;
    virtual void processSupply(Building& t_building, int t_supply) override;
    virtual void interact(Building& t_building) override;
};

