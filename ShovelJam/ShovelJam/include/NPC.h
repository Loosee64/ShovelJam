#pragma once
#include "GameObject.h"
#include "Bullet.h"

class NPC :
    public GameObject
{
public:
    NPC();
    virtual void init() override;
    virtual void movement() override;

    void update(Vector2 t_target);
    void draw();
    void startFollowing() { m_following = true; }
    void spawn(Vector2 t_start);

    void findTarget(Vector2* t_targets, int t_maxTargets);
    void shoot(Vector2 t_target);

    int getActiveBullet() { return m_activeBullet; }
    Vector2 getBulletPos() { return bullets[m_activeBullet].getPosition(); }
    float getBulletRadius() { return bullets[m_activeBullet].getRadius(); }
    void resetBullet();

private:
    static const int MAX_BULLETS = 5;
    Bullet bullets[5];
    std::string m_name;

    Vector2 m_target;

    bool m_following;
    bool m_active;
    int m_activeBullet;
    float dt;
    float m_shootingCooldown;

    int m_maxHealth;
};

