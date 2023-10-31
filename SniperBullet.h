//Name: Grant Clark
//File: SniperBullet.h

#ifndef SNIPER_BULLET_H
#define SNIPER_BULLET_H

#include "Bullet.h"

class SniperBullet : public Bullet
{
public:
    SniperBullet(const float x, const float y,
                 const float degrees) :
        Bullet(4, x, y)
    {
        setFillColor(sf::Color::White);

        // Calculate the dx and dy from the direction the
        // circle is facing (12 is the number of pixels
        // moved per frame).
        dx() = cos(degrees * (M_PI / 180.0)) * 18;
        dy() = sin(degrees * (M_PI / 180.0)) * 18;
    }

    ~SniperBullet(){}
};

#endif
