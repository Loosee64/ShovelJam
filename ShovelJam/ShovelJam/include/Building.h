#pragma once
#include "GameObject.h"
#include "BuildingType.h"
#include "SupplyShed.h"
#include "Barracks.h"
#include "Bunks.h"
#include <string>

class Building :
    public GameObject
{
public:
    friend class SupplyShed;
    friend class Barracks;
    friend class Bunks;

    Building(std::shared_ptr<BuildingType> t_type);
    virtual void init() override;
    virtual void update() override;

    Rectangle getBody() { return m_body; }
    int returnValue() { return m_remainder; }
    int subractValue() { return m_subtract; }
    bool completed() { return m_isComplete; }
    std::string getName() { return m_name; }

    void draw();
    void spawn(Vector2 t_pos);
    void build(int t_valueApplied);
    void interact(int t_value);

private:
    Rectangle m_body;
    std::string displayText;
    std::shared_ptr<BuildingType> m_type;

    bool m_isComplete;

    std::string m_name;
    int m_time;
    int m_value;
    int m_currentValue;
    int m_remainder;
    int m_subtract;
    float m_width;
    float m_height;
};

