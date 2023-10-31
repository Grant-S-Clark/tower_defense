//Name: Grant Clark
//File: Bullet.h

#ifndef BULLET_H
#define BULLET_H

#include "Global.h"

class Bullet : public sf::CircleShape
{
public:
    Bullet(const float radius,
           const float x, const float y,
           const float dx = 0, const float dy = 0) :
        sf::CircleShape(radius), dx_(dx), dy_(dy)
    {
        setOrigin(radius, radius);
        setPosition(x, y);
    }

    virtual ~Bullet(){};

    float & dx() { return dx_; }
    float dx() const { return dx_; }
    
    float & dy() { return dy_; }
    float dy() const { return dy_; }
    
    void move();
private:
    float dy_, dx_;
};

#endif
