//Name: Grant Clark
//File: Path.cpp

#include "Path.h"

sf::RenderWindow * Path::window_ = nullptr;

void Path::set_window(sf::RenderWindow * const & window)
{ window_ = window; }


void Path::generate(const int scale_factor)
{
    path_.clear();

    //Starting point that is off screen.
    path_.push_back(DirectedPoint());
    path_.back().p.x = -off_screen_;
    path_.back().p.y = rand() % (max_y_ - lower_bound_ * 2)
        + min_y_ + upper_bound_;
    path_.back().dir = Forward;

    //Point after starting point on same y-position that
    //starts from x = start_x_.
    path_.push_back(DirectedPoint());
    path_.back().p.x = start_x_;
    path_.back().p.y = path_[0].p.y;
    path_.back().dir = Forward;

    int pos = start_x_;

    int height = max_y_ - min_y_ - lower_bound_ - upper_bound_;
    int width = end_x_ - start_x_;
    
    Direction dir = Forward, old_dir;
    while (path_.back().p.x < end_x_)
    {
        int dx, dy;
        DirectedPoint next = path_.back();
        bool valid_point = true;

        old_dir = dir;
        switch (dir)
        {
            //Up
            case Up:
                //Decide on how much to move.
                dx = 0;
                dy = rand() % (width / scale_factor - 50) + 25;
                dy *= -1;

                //Make new direction.
                if (rand() % 2)
                    dir = Up;
                else
                    dir = Forward;
                
                break;

            //Forward
            case Forward:
                dy = 0;
                dx = rand() % (height / scale_factor - 50) + 25;

                dir = Direction(rand() % 3 - 1);
                
                break;
           
            //Down
            case Down:
                dx = 0;
                dy = rand() % (width / scale_factor - 50) + 25;

                if (rand() % 2)
                    dir = Down;
                else
                    dir = Forward;
                
                break;
        }

        next.p.x += dx;
        next.p.y += dy;
        
        //Went off of top or bottom of screen.
        if (next.p.y < min_y_ + upper_bound_ ||
            next.p.y > max_y_ - lower_bound_)
        {
            valid_point = false;

            dir = Forward;
        }

        if (valid_point)
        {
            if (next.p.x >= end_x_)
                next.p.x = end_x_ + off_screen_;

            next.dir = old_dir;
            path_.push_back(next);
        }
    }

    //Ending point to signal balloon deletion.
    path_.push_back(DirectedPoint());
    path_.back().p = path_.at(path_.size() - 2).p;
    path_.back().dir = End;
    
    return;
}


void Path::draw() const
{
    if (window_ == nullptr)
        throw NullWindowError();
    
    //Set the window as the window to recieve OpenGL
    //draw calls.
    window_->draw(gl_helper);

    glColor3ub(0, 255, 0);
    glBegin(GL_LINES);
    // Loop starts at 2 as the first line is off screen and
    // thus there is no need to draw it. It also stops
    // at n - 1 as due to the dummy ending point being
    // placed at the end of the path.
    for (int i = 2, n = path_.size(); i < n - 1; ++i)
    {
        glVertex2i(path_.at(i - 1).p.x, path_.at(i - 1).p.y);
        glVertex2i(path_.at(i).p.x, path_.at(i).p.y);
    }
    glEnd();

    return;
}
