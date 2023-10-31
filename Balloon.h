//Name: Grant Clark
//File: Balloon.h

#ifndef BALLOON_H
#define BALLOON_H

#include "Global.h"
#include "Path.h"

class Balloon : public sf::CircleShape
{
public:
    Balloon(const int radius) :
        sf::CircleShape(radius),
        path_index_(0),
        speed_((float)rand() / RAND_MAX * 2 + 1),
        end_of_path_(false)
    {
        if (path_ == nullptr)
            throw NullPathError();

        setFillColor(sf::Color(rand() % 256,
                               rand() % 206 + 50,
                               rand() % 256));
        setOrigin(radius, radius);
        setOutlineColor(sf::Color::Red);
        setOutlineThickness(1);

        setPosition(path_->at(0).p.x, path_->at(0).p.y);
    }
    
    ~Balloon(){}
    
    void follow_path();

    float speed() const { return speed_; }
    int path_index() const { return path_index_; }
    bool end_of_path() const { return end_of_path_; }

    static void set_path(Path * const &);
private:
    int path_index_;
    float speed_;
    bool end_of_path_;                 
    static Path * path_;
};

#endif
