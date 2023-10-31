//Name: Grant Clark
//File: SniperTurret.h

#ifndef SNIPER_TURRET_H
#define SNIPER_TURRET_H

#include "Turret.h"
#include "SniperBullet.h"

class SniperTurret : public Turret
{
public:
    //Body radius 12, shoot radius / range 300,
    //shoot frame of 180, and position of (x, y).
    //"Sniper" turret type
    SniperTurret(const float x = 0, const float y = 0) :
        Turret(12, 300, 140, x, y, Sniper)
    {
        body().setFillColor(sf::Color::Blue);
        body().setOutlineThickness(1);
        body().setOutlineColor(sf::Color::White);

        barrel_setup();

        return;
    }

    ~SniperTurret(){}

    //Barrel setup for a sniper turret
    void barrel_setup();

    void aim_at(const Balloon & b);
    std::vector< Bullet* > fire();
};

#endif
