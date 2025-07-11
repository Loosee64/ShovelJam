#pragma once
#include "GameObject.h"
#include "Bullet.h"

class Player :
    public GameObject
{
public:
    Player();
    virtual void init();
    virtual void update();
    virtual void movement();

    void draw();

    void input();
    void shoot();

    Vector2 getBulletPos() { return bullets[activeBullet].getPosition(); }
    float getBulletRadius() { return bullets[activeBullet].getRadius(); }
    void resetBullet();

private:
    static const int MAX_BULLETS = 10;
    Bullet bullets[MAX_BULLETS];
    int activeBullet;
};

