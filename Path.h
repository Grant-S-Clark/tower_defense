//Name: Grant Clark
//File: Path.h

#ifndef PATH_H
#define PATH_H

#include "Global.h"

struct Point
{
    int x;
    int y;
};

enum Direction {
    Forward = 0,
    Down = 1,
    Up = -1,
    End = -999 //Dummy value
};

struct DirectedPoint
{
    Point p;
    Direction dir;
};

class Path
{
public:
    Path(const int start_x, const int end_x,
         const int min_y, const int max_y,
         const int upper_bound = BOUNDING,
         const int lower_bound = BOUNDING,
         const int off_screen = OFF_SCREEN) :
        start_x_(start_x), end_x_(end_x),
        min_y_(min_y), max_y_(max_y),
        upper_bound_(upper_bound), lower_bound_(lower_bound),
        off_screen_(off_screen),
        gl_helper(0)
    {}

    ~Path(){}

    void generate(const int scale_factor = 5);
    void draw() const;

    int size() const { return path_.size(); }

    DirectedPoint operator[](const int i) const
    { return path_.at(i); }
    DirectedPoint & operator[](const int i)
    { return path_.at(i); }
    DirectedPoint at(const int i) const
    { return path_.at(i); }
    DirectedPoint & at(const int i)
    { return path_.at(i); }

    static void set_window(sf::RenderWindow * const &);
private:
    std::vector< DirectedPoint > path_;

    int start_x_, end_x_;
    int min_y_, max_y_;
    int upper_bound_, lower_bound_;
    int off_screen_;

    sf::CircleShape gl_helper;

    static sf::RenderWindow * window_;
};

#endif
