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
    virtual void draw();
    virtual void movement();

    void input();
    void shoot();

    Vector2 getBulletPos() { return bullet.getPosition(); }
    float getBulletRadius() { return bullet.getRadius(); }
    void resetBullet();

private:
    Bullet bullet;
};

