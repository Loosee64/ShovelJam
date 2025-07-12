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
    virtual void kill() override;
    virtual void damage() override;

    void draw();

    void input();
    void shoot();

    int getActiveBullet() { return activeBullet; }
    Vector2 getBulletPos() { return bullets[activeBullet].getPosition(); }
    float getBulletRadius() { return bullets[activeBullet].getRadius(); }
    void resetBullet();

private:
    static const int MAX_BULLETS = 10;
    const int MAX_IFRAMES = 90;
    Bullet bullets[MAX_BULLETS];
    int activeBullet;
    int iFrames;
};

