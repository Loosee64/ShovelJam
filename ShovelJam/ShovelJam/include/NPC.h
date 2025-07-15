#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include "NPCBehaviour.h"
#include "OffensiveBehaviour.h"
#include "DefensiveBehaviour.h"

class NPC :
    public GameObject
{
    friend class OffensiveBehaviour;
    friend class DefensiveBehaviour;

public:
    NPC(std::string t_name, Vector2 t_pos, std::shared_ptr<NPCBehaviour> t_behaviour);
    ~NPC();
    virtual void init() override;
    virtual void movement() override;

    void update(Vector2 t_target);
    void draw();
    void startFollowing() { m_following = true; }
    void stopFollowing() { m_following = false; }
    void spawn(Vector2 t_start);

    bool isFollowing() { return m_following; }
    bool isBuilding() { return m_building; }

    void findTarget(Vector2* t_targets, int t_maxTargets);
    void shoot(Vector2 t_target);
    void approachTarget();
    void passive();
    void resetTarget() { m_target = m_position; }
    void exitPassive();

    void assignToBuilding(Vector2 t_pos);
    void removeFromBuilding();

    void newArea(Cell t_direction, Vector2 t_start);

    std::string getName() { return m_name; }

    int getActiveBullet() { return m_activeBullet; }
    Vector2 getBulletPos() { return bullets[m_activeBullet].getPosition(); }
    float getBulletRadius() { return bullets[m_activeBullet].getRadius(); }
    void resetBullet();


private:
    static const int MAX_BULLETS = 5;
    const float MAX_SPEED = 3.5f;
    Bullet bullets[5];
    std::string m_name;

    Vector2 m_target;
    Vector2 m_newAreaTarget;

    bool m_following;
    bool m_building;
    int m_activeBullet;
    float dt;

    float areaTimer;
    float areaTimerMax;

    float m_shootingCooldown;

    int m_maxHealth;
    float approachDistance;

    std::shared_ptr<NPCBehaviour> behaviour;
};

