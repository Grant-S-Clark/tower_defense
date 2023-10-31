//Name: Grant Clark
//File: Balloon.cpp

#include "Balloon.h"

Path * Balloon::path_ = nullptr;


void Balloon::set_path(Path * const & path)
{ path_ = path; }


void Balloon::follow_path()
{
    if (path_ == nullptr)
        throw NullPathError();

    DirectedPoint * point = &path_->at(path_index_);
    DirectedPoint * next = &path_->at(path_index_ + 1);
    
    float diff;
    bool too_far = false;
    switch(point->dir)
    {
        case Forward:
            move(speed_, 0);
            if (getPosition().x > next->p.x)
            {
                diff = next->p.x - getPosition().x;
                move(diff, 0);
                ++path_index_;
                if (point->dir != Forward)
                    too_far = true;
            }
            break;
        case Up:
            move(0, -speed_);
            if (getPosition().y < next->p.y)
            {
                diff = next->p.y - getPosition().y;
                move(0, diff);
                ++path_index_;
                if (point->dir != Up)
                    too_far = true;
            }
            break;
        case Down:
            move(0, speed_);
            if (getPosition().y > next->p.y)
            {
                diff = next->p.y - getPosition().y;
                move(0, diff);
                ++path_index_;
                if (point->dir != Down)
                    too_far = true;
            }
            break;
    }

    //Adjust if you moved off of the path and move
    //the difference in the correct direction.
    if (too_far)
    {
        switch (next->dir)
        {
            case Forward:
                move(diff, 0);
                break;
            case Up:
                move(0, -diff);
                break;
            case Down:
                move(0, diff);
                break;
        }
    }

    // Mark balloon for deletion if it has reached
    // the end of the path.
    else if (path_->at(path_index_).dir == End)
        end_of_path_ = true;
    
    return;
}
