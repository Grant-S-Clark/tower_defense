//Name: Grant Clark
//File: SpawnRect.cpp

#include "SpawnRect.h"

sf::RenderWindow * SpawnRect::window_ = nullptr;
std::vector< Turret* > * SpawnRect::turrets_ = nullptr;
Path * SpawnRect::path_ = nullptr;


void SpawnRect::set_window(sf::RenderWindow * const & window)
{ window_ = window; }
void SpawnRect::set_turrets(std::vector< Turret* > * const & turrets)
{ turrets_ = turrets; }
void SpawnRect::set_path(Path * const & path)
{ path_ = path; }


void SpawnRect::setPosition(const float x, const float y)
{
    rectangle_.setPosition(x, y);
    turret_->setPosition(x, y);
}

void SpawnRect::draw()
{
    if (window_ == nullptr)
        throw NullWindowError();

    window_->draw(rectangle_);

    //Draw every part of turret EXCEPT the range circle.
    turret_->draw();
    
    if (hover_turret_ != nullptr)
    {
        hover_turret_->draw_range();
        hover_turret_->draw();
        window_->draw(*valid_indicator_);
    }
    
    return;
}


bool SpawnRect::intersects(const sf::FloatRect & rect) const
{
    return rectangle_.getGlobalBounds().intersects(rect);
}


void SpawnRect::update()
{
    if (window_ == nullptr)
        throw NullWindowError();

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window_);
            
    if (!is_clicking_ && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        is_clicking_ = true;
        
        if (!dragging_ &&
            rectangle_.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
        {
            dragging_ = true;
        }
    }

    else if (is_clicking_ && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        is_clicking_ = false;
        if (dragging_)
        {
            dragging_ = false;
            if (hover_turret_ != nullptr)
            {
                delete hover_turret_;
                hover_turret_ = nullptr;
                delete valid_indicator_;
                valid_indicator_ = nullptr;
            }
            
            if (valid_turret_position(mouse_pos))
            {
                //use a boolean to tell the main loop a turret
                //has been placed.
                place_turret_ = true;
                turret_pos_ = sf::Mouse::getPosition(*window_);
            }
        }
    }

    //Update hovering turret.
    if (dragging_)
    {
        adjust_hover_turret(mouse_pos);
    }
    
    return;
}


Turret* SpawnRect::get_new_turret()
{
    Turret* ret = nullptr;

    if (place_turret_)
    {
        switch(turret_->type())
        {
            case Basic:
                ret = new BasicTurret(turret_pos_.x, turret_pos_.y);
                break;
            case Sniper:
                ret = new SniperTurret(turret_pos_.x, turret_pos_.y);
                break;
        }
    }

    place_turret_ = false;
    
    return ret;
}

///Private Functions

void SpawnRect::adjust_hover_turret(const sf::Vector2i & mouse_pos)
{
    //If not created yet, make the hover turret.
    if (hover_turret_ == nullptr)
    {
        //Set pointer.
        switch (turret_->type())
        {
            case Basic:
                hover_turret_ = new BasicTurret(0, 0);
                break;
            case Sniper:
                hover_turret_ = new SniperTurret(0, 0);
                break;
        }

        ///Set half transparency.
        //Body
        sf::Color c = hover_turret_->body().getFillColor();
        c.a = c.a / 2;
        hover_turret_->body().setFillColor(c);

        //Barrels.
        c = hover_turret_->barrels().at(0).getFillColor();
        c.a = c.a / 2;
        for (sf::RectangleShape & rect : hover_turret_->barrels())
            rect.setFillColor(c);

        //Radius Circle
        c = hover_turret_->range_circle().getFillColor();
        c.a = c.a / 2;
        hover_turret_->range_circle().setFillColor(c);

        //Validity indicator circle
        valid_indicator_ = new sf::CircleShape(
            hover_turret_->range_circle().getRadius()
            );
        valid_indicator_->setOrigin(valid_indicator_->getRadius(),
                                    valid_indicator_->getRadius());
    }

    //set position.
    hover_turret_->setPosition(mouse_pos.x, mouse_pos.y);
    valid_indicator_->setPosition(mouse_pos.x, mouse_pos.y);

    sf::Color c(255, 0, 0, 0);
    if (!valid_turret_position(mouse_pos))
        c.a = 75;
    valid_indicator_->setFillColor(c);
    
    return;
}

#include <iostream> //AAAAAAAAAAAAAAAAAa
bool SpawnRect::valid_turret_position(const sf::Vector2i & mouse_pos) const
{
    if (turrets_ == nullptr)
        throw NullTurretListError();
    if (path_ == nullptr)
        throw NullPathError();
    
    sf::FloatRect turret_rect, level_rect;
        
    turret_rect.width = turret_->barrels().at(0).getSize().y * 2;
    turret_rect.height = turret_rect.width;
    turret_rect.top = mouse_pos.y - turret_rect.height / 2;
    turret_rect.left = mouse_pos.x - turret_rect.width / 2;

    level_rect.top = 0;
    level_rect.left = 0;
    level_rect.width = LEVEL_WIDTH;
    level_rect.height = HEIGHT;

    // If the turret rect is not within the play area.
    if (!level_rect.contains(turret_rect.left, turret_rect.top) ||
        !level_rect.contains(turret_rect.left + turret_rect.width,
                             turret_rect.top + turret_rect.height))
    {
        return false;
    }

    sf::CircleShape test(turret_->barrels().at(0).getSize().y);
    test.setOrigin(test.getRadius(), test.getRadius());
    test.setPosition(mouse_pos.x, mouse_pos.y);
    // If the turret intersects with any other turrets.
    for (int i = 0, n = turrets_->size(); i < n; ++i)
        if (circle_collision(test, turrets_->at(i)->body()))
            return false;
    
    // If it intersects with any lines in the path.
    sf::FloatRect line_rect;
    DirectedPoint start, end;
    int n = path_->size();
    for (int i = 1; i < n - 2; ++i)
    {
        start = path_->at(i);
        end = path_->at(i + 1);

        //NOTE, THE DIRECTION THE PREVIOUS POINT IS HEADING IS
        //STORED IN THE NEXT POINT'S DIRECTION VARIABLE.
        switch (end.dir)
        {
            case Forward:
                line_rect.top = start.p.y;
                line_rect.left = start.p.x;
                line_rect.height = 1;
                line_rect.width = end.p.x - start.p.x;
                break;
            case Up:
                line_rect.top = end.p.y;
                line_rect.left = start.p.x;
                line_rect.height = start.p.y - end.p.y;
                line_rect.width = 1;
                break;
            case Down:
                line_rect.top = start.p.y;
                line_rect.left = start.p.x;
                line_rect.height = end.p.y - start.p.y;
                line_rect.width = 1;
                break;
        }

        if (test.getGlobalBounds().intersects(line_rect))
            return false;
    }
    
    return true;
}
