#pragma once
#include "GameObject.h"
#include <string>

class Building :
    public GameObject
{
public:
    Building(std::string t_type, int t_time, int t_value);
    virtual void init() override;
    virtual void update() override;

    Rectangle getBody() { return m_body; }
    int returnValue() { return m_remainder; }
    bool completed() { return m_isComplete; }
    std::string getType() { return m_type; }

    void draw();
    void spawn(Vector2 t_pos);
    void build(int t_valueApplied);

private:
    Rectangle m_body;
    std::string displayText;

    bool m_isComplete;

    std::string m_type;
    int m_time;
    int m_value;
    int m_currentValue;
    int m_remainder;
    float m_width;
    float m_height;
};

