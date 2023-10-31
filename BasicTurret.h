//Name: Grant Clark
//File: BasicTurret.h

#ifndef BASIC_TURRET_H
#define BASIC_TURRET_H

#include "Turret.h"
#include "BasicBullet.h"

class BasicTurret : public Turret
{
public:
    // Body radius 15, shoot radius / range 150,
    // shoot frame of 60, and position of (x, y).
    // "Basic" type turret.
    BasicTurret(const float x = 0, const float y = 0) :
        Turret(15, 150, 60, x, y, Basic)
    {
        body().setFillColor(sf::Color::Green);
        body().setOutlineThickness(1);
        body().setOutlineColor(sf::Color::White);

        barrel_setup();

        return;
    }

    ~BasicTurret(){}
    
    //Barrel Setup for a basic turret.
    void barrel_setup();

    void aim_at(const Balloon & b);
    std::vector< Bullet* > fire();
};

#endif
