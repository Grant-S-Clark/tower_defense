//Name: Grant Clark
//File: Turret.h

#include "Turret.h"

sf::RenderWindow * Turret::window_ = nullptr;

// Returns a vector of all balloons withing the firing range
// of a given turret.
std::vector< Balloon* >
Turret::balloons_in_range(const std::vector< Balloon* > & balloons) const
{
    std::vector< Balloon* > ret;

    for (Balloon * const & b : balloons)
    {
        if (circle_collision(range_circle_, *b))
            ret.push_back(b);
    }
    
    return ret;
}

void Turret::draw()
{
    if (window_ == nullptr)
        throw NullWindowError();

    for (int i = 0, n = barrels_.size(); i < n; ++i)
        window_->draw(barrels_[i]);
    window_->draw(body_);

    // Increment the frame counter if it is
    // less than the shooting frame.
    if (frame_ < shoot_frame_)
        ++frame_;
    
    return;
}

void Turret::draw_range() const
{
    window_->draw(range_circle_);
}


void Turret::setPosition(const float x, const float y)
{
    range_circle_.setPosition(x, y);
    for (int i = 0, n = barrels_.size(); i < n; ++i)
        barrels_[i].setPosition(x, y);
    body_.setPosition(x, y);
}


void Turret::rotate(const float degrees)
{
    for (int i = 0, n = barrels_.size(); i < n; ++i)
        barrels_[i].rotate(degrees);
    body_.rotate(degrees);
}


void Turret::setRotation(const float degrees)
{
    for (int i = 0, n = barrels_.size(); i < n; ++i)
        barrels_[i].setRotation(degrees);
    body_.setRotation(degrees);
}


void Turret::set_window(sf::RenderWindow * const & window)
{ window_ = window; }

