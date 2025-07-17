#pragma once
#include "GameObject.h"
#include "BuildingType.h"
#include "SupplyShed.h"
#include "Barracks.h"
#include "Bunks.h"
#include "NPC.h"
#include <string>

class Building :
    public GameObject
{
public:
    friend class SupplyShed;
    friend class Barracks;
    friend class Bunks;

    Building(std::shared_ptr<BuildingType> t_type, int t_sprite);
    virtual void init() override;
    virtual void update() override;
    void reset();

    Rectangle getBody() { return m_body; }
    int returnValue() { return m_remainder; }
    int subractValue() { return m_subtract; }
    bool inProcess() { return m_inProcess; }
    bool completed() { return m_isComplete; }
    std::string getName() { return m_name; }

    void forceComplete() { m_isComplete = true; }
    void disableText() { m_textDisplay = false; }

    void draw();
    void spawn(Vector2 t_pos);
    void build(int t_valueApplied);
    void assignBuilder(std::shared_ptr<NPC> t_npc, int t_time) { m_builder = t_npc; m_timeStarted = t_time; }
    void removeBuilder();
    bool hasBuilder();

    void applyProgress();

    void completeCheck(int t_time);
    void interact(int t_value);

private:
    Rectangle m_body;
    std::string displayText;
    std::shared_ptr<BuildingType> m_type;

    std::shared_ptr<NPC> m_builder;

    bool m_inProcess;
    bool m_isComplete;
    bool m_textDisplay;

    int m_sprite;

    std::string m_name;
    int m_time;
    int m_timeStarted;
    int m_timeLeft;
    int m_value;
    int m_currentValue;
    int m_remainder;
    int m_subtract;
    float m_width;
    float m_height;
};

