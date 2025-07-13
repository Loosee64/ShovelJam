#pragma once
#include "GameObject.h"
#include "Bullet.h"

class Player :
    public GameObject
{
public:
    Player();
    virtual void init() override;
    virtual void update() override;
    virtual void movement() override;
    virtual void damage() override;

    void draw();
    void kill();

    void input();
    void shoot();

    void setPositionX(float t_x) { m_position.x = t_x; }
    void setPositionY(float t_y) { m_position.y = t_y; }

    int getActiveBullet() { return activeBullet; }
    Vector2 getBulletPos() { return bullets[activeBullet].getPosition(); }
    float getBulletRadius() { return bullets[activeBullet].getRadius(); }
    void resetBullet();

    void addSupply(int t_val) { supplyValue += t_val; }
    void subtractSupply(int t_val) { supplyValue -= t_val; }
    int currentSupply() { return supplyValue; }

private:
    static const int MAX_BULLETS = 10;
    const int MAX_IFRAMES = 0.5f;
    Bullet bullets[MAX_BULLETS];
    int activeBullet;
    int iFrames;
    float dt;
    float m_shootingCooldown;
    int supplyValue;
};

