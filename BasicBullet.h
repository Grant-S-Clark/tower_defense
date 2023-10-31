//Name: Grant Clark
//File: BasicBullet.h

#ifndef BASIC_BULLET_H
#define BASIC_BULLET_H

#include "Bullet.h"

class BasicBullet : public Bullet
{
public:
    BasicBullet(const float x, const float y,
                const float degrees) :
        Bullet(4, x, y)
    {
        setFillColor(sf::Color::White);

        // Calculate the dx and dy from the direction the
        // circle is facing (5 is the number of pixels
        // moved per frame).
        dx() = cos(degrees * (M_PI / 180.0)) * 5;
        dy() = sin(degrees * (M_PI / 180.0)) * 5;
    }

    ~BasicBullet(){}
};

#endif
