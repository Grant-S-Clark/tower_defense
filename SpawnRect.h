//Name: Grant Clark
//File: SpawnRect

#ifndef SPAWN_RECT_H
#define SPAWN_RECT_H

#include "Global.h"
#include "Path.h"
#include "Turret.h"
#include "BasicTurret.h"
#include "SniperTurret.h"

class SpawnRect
{
public:
    SpawnRect(const float width, const float height,
              const float x, const float y,
              const Type type) :
        rectangle_(sf::Vector2f(width, height)),
        is_clicking_(false), dragging_(false),
        place_turret_(false), hover_turret_(nullptr)
    {
        rectangle_.setOrigin(width / 2, height / 2);
        rectangle_.setPosition(x, y);
        rectangle_.setFillColor(sf::Color::Black);
        rectangle_.setOutlineThickness(2);
        rectangle_.setOutlineColor(sf::Color(125, 125, 125));
        switch (type)
        {
            case Basic:
                turret_ = new BasicTurret(x, y);
                break;
            case Sniper:
                turret_ = new SniperTurret(x, y);
                break;
        }

        turret_->setPosition(x,
                             y + (turret_->barrels().at(0).getSize().y - turret_->body().getRadius()) / 4);

        for (int i = 0, n = turret_->barrels().size();
             i < n;
             ++i)
        {
            turret_->barrels().at(i).scale(0.5, 0.5);
        }
        turret_->body().scale(0.5, 0.5);
    }

    ~SpawnRect()
    {
        if (turret_ != nullptr)
            delete turret_;
        if (hover_turret_ != nullptr)
        {
            delete hover_turret_;
            delete valid_indicator_;
        }
    }

    void setPosition(const float, const float);
    void draw();

    bool intersects(const sf::FloatRect &) const;

    void update();

    Turret* get_new_turret();
    
    static void set_window(sf::RenderWindow * const &);
    static void set_turrets(std::vector< Turret* > * const &);
    static void set_path(Path * const &);
private:
    void adjust_hover_turret(const sf::Vector2i &);
    bool valid_turret_position(const sf::Vector2i &) const;
    
    sf::RectangleShape rectangle_;
    Turret * turret_;
    Turret * hover_turret_;
    sf::CircleShape * valid_indicator_;

    bool is_clicking_;
    bool dragging_;
    bool place_turret_;

    sf::Vector2i turret_pos_;
    
    static sf::RenderWindow * window_;
    static std::vector< Turret* > * turrets_;
    static Path * path_;
};

#endif
