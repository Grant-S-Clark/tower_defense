//Name: Grant Clark
//File: Turret.h

#ifndef TURRET_H
#define TURRET_H

#include "Global.h"
#include "Balloon.h"
#include "Bullet.h"

enum Type
{
    Basic,
    Sniper
};

class Turret
{
public:
    Turret(const float radius, const float range,
           const int shoot_frame,
           const float x, const float y,
           const Type type) :
        body_(radius), range_circle_(range), range_(range),
        frame_(shoot_frame), shoot_frame_(shoot_frame),
        type_(type), aiming_(false)
    {
        body_.setOrigin(radius, radius);
        body_.setPosition(x, y);
        range_circle_.setOrigin(range, range);
        range_circle_.setPosition(x, y);
        
        //Set color of range to a transparent gray.
        range_circle_.setFillColor(sf::Color(125, 125, 125, 50));
    }

    virtual ~Turret()
    {}

    //Pure static
    virtual void barrel_setup() = 0;
    virtual void aim_at(const Balloon & b) = 0;
    virtual std::vector< Bullet* > fire() = 0;

    std::vector< Balloon* > balloons_in_range(const std::vector< Balloon* > &) const;

    sf::CircleShape & body() { return body_; }
    sf::CircleShape body() const { return body_; }

    sf::CircleShape & range_circle() { return range_circle_; }
    sf::CircleShape range_circle() const { return range_circle_; }

    float range() const { return range_; }

    Type type() const { return type_; }
    
    bool & aiming() { return aiming_; }
    bool aiming() const { return aiming_; }

    int & frame() { return frame_; }
    int frame() const { return frame_; }
    
    const int shoot_frame() const { return shoot_frame_; }
    
    std::vector< sf::RectangleShape > & barrels()
    { return barrels_; }
    std::vector< sf::RectangleShape > barrels() const
    { return barrels_; }

    void draw();
    void draw_range() const;
    void setPosition(const float, const float);
    void move(const float, const float);
    void rotate(const float);
    void setRotation(const float);
    
    static void set_window(sf::RenderWindow * const &);
private:
    sf::CircleShape body_, range_circle_;
    std::vector< sf::RectangleShape > barrels_;
    const float range_;
    int frame_;
    bool aiming_;
    const int shoot_frame_;
    Type type_;
    
    static sf::RenderWindow * window_;
};

#endif
