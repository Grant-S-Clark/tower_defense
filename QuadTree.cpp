//Name: Grant Clark
//File: QuadTree.cpp

#include "QuadTree.h"

void QuadTree::update()
{
    if (root_ != nullptr)
        delete root_;

    root_ = new QuadTreeNode(0, x_, y_,
                             width_, height_);

    root_->set_bullets(*bullets_);
    root_->set_balloons(*balloons_);

    root_->distribute(threshold_, max_depth_);

    return;
}


std::vector< std::vector< sf::CircleShape* > > QuadTree::get_collisions() const
{
    std::vector< std::vector< sf::CircleShape* > > collisions;

    root_->get_collisions(collisions);

    return collisions;
}
