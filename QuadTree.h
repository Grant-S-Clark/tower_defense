//Name: Grant Clark
//File: QuadTree.h

#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "Global.h"
#include "QuadTreeNode.h"

class QuadTree
{
public:
    QuadTree(int threshold, int max_depth,
             float x, float y, float width, float height,
             std::vector< Bullet* > * const bullets,
             std::vector< Balloon* > * const balloons) :
        root_(nullptr),
        threshold_(threshold), max_depth_(max_depth),
        x_(x), y_(y),
        width_(width), height_(height),
        bullets_(bullets), balloons_(balloons)
    {}

    ~QuadTree()
    {
        if (root_ != nullptr)
            delete root_;

        return;
    }

    void update();
    std::vector< std::vector< sf::CircleShape* > > get_collisions() const;
    
private:
    int threshold_;
    int max_depth_;

    float x_, y_;
    float width_, height_;

    std::vector< Bullet* > * bullets_;
    std::vector< Balloon* > * balloons_;

    QuadTreeNode * root_;
};

#endif
