//Name: Grant Clark
//File: SniperTurret.cpp

#include "SniperTurret.h"

void SniperTurret::barrel_setup()
{
    //Make rectangle.
    //Grabs radius to get rectangle to edge, then gets
    //it to stick out by 10 pixels.
    barrels().push_back(
        sf::RectangleShape(sf::Vector2f(8, body().getRadius() + 30))
        );
    //Set colors.
    barrels().back().setFillColor(
        sf::Color(125, 125, 125)
        );
    barrels().back().setOutlineThickness(1);
    barrels().back().setOutlineColor(
        body().getOutlineColor()
        );

    //Origin (located at bottom line or rect at midpoint).
    barrels().back().setOrigin(
        barrels().back().getSize().x / 2,
        barrels().back().getSize().y
        );
    
    //Positioning.
    barrels().back().setPosition(
        body().getPosition().x,
        body().getPosition().y
        );
    
    return;
}


void SniperTurret::aim_at(const Balloon & b)
{
    float adjacent = body().getPosition().x - b.getPosition().x;
    float opposite = body().getPosition().y - b.getPosition().y;

    float angle = atan2(opposite, adjacent) * (180.0 / M_PI);

    //Subtract 90.0 becasue default angle 0.0 is to the right.
    setRotation(angle - 90.0);

    aiming() = true;
    
    return;
}


std::vector< Bullet* > SniperTurret::fire()
{
    std::vector< Bullet* > ret;

    //TESTING TIMER
    if (frame() == shoot_frame() && aiming())
    {
        // Reset values.
        frame() = 0;
        aiming() = false;

        ///Shoot bullet in direction you are aiming.
        //offsets used for spawning the bullet at the end of
        //the turret barrel instead at the center of the turret.
        float angle_radians = (body().getRotation() - 90) /
            (180.0 / M_PI);
        float offset_x = cos(angle_radians) *
            barrels().at(0).getSize().y;
        float offset_y = sin(angle_radians) *
            barrels().at(0).getSize().y;
        ret.push_back(new SniperBullet(body().getPosition().x + offset_x,
                                       body().getPosition().y + offset_y,
                                       body().getRotation() - 90)
            );
    }
    
    return ret;
}
