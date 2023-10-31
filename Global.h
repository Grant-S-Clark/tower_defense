//Name: Grant Clark
//File: Global.h

#ifndef GLOBAL_H
#define GLOBAL_H

#include <unordered_set>
#include <vector>
#include <cstdlib>
#include <ctime>

#define _USE_MATH_DEFINES //for M_PI
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

const int WIDTH = 1020;
const int HEIGHT = 640;
const int BOUNDING = 50;
const int OFF_SCREEN = 50;
const int MENU_WIDTH = 200;
const int LEVEL_WIDTH = WIDTH - MENU_WIDTH;

//Error throwing class.
class NullWindowError{};
class NullPathError{};
class NullTurretListError{};

// inline circle collision function. MAKE WORK FOR ENVELOPING.
inline
bool circle_collision(const sf::CircleShape & c1,
                      const sf::CircleShape & c2)
{
    float r1 = c1.getRadius(), r2 = c2.getRadius();
    sf::Vector2f size1 = c1.getPosition(), size2 = c2.getPosition();

    return r1 + r2 >=
        sqrt(
            (size2.x - size1.x) * (size2.x - size1.x) +
            (size2.y - size1.y) * (size2.y - size1.y)
            );
}

inline
bool off_screen(const sf::CircleShape & c)
{
    float radius = c.getRadius();
    sf::Vector2f pos = c.getPosition();

    return pos.x + radius < 0 || pos.x - radius > WIDTH ||
        pos.y + radius < 0 || pos.y - radius > HEIGHT;
}

#endif
