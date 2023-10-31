//Name: Grant Clark
//File: QuadTreeNode.h

#ifndef QUAD_TREE_NODE_H
#define QUAD_TREE_NODE_H

#include "Global.h"
#include "Bullet.h"
#include "Balloon.h"

class QuadTreeNode
{
public:
    QuadTreeNode(const int depth,
                 const float left, const float top,
                 const float width, const float height,
                 QuadTreeNode* parent = nullptr) :
        depth_(depth),
        rect_(sf::FloatRect(left, top, width, height)),
        parent_(parent),
        has_children_(false)
    {
        children_[0] = nullptr;
        children_[1] = nullptr;
        children_[2] = nullptr;
        children_[3] = nullptr;
    }

    ~QuadTreeNode()
    {
        if (children_[0] != nullptr)
            delete children_[0];
        if (children_[1] != nullptr)
            delete children_[1];
        if (children_[2] != nullptr)
            delete children_[2];
        if (children_[3] != nullptr)
            delete children_[3];
    }

    bool & has_children() { return has_children_; }
    bool has_children() const { return has_children_; }

    inline
    void set_bullets(const std::vector< Bullet* > & bullets)
    { bullets_ = bullets; }

    inline
    void set_balloons(const std::vector< Balloon* > & balloons)
    { balloons_ = balloons; }

    void distribute(const int threshold, const int max_depth = -1);

    void get_collisions(std::vector< std::vector< sf::CircleShape* > > &) const;
private:
    int depth_;
    
    sf::FloatRect rect_;
    bool has_children_;

    std::vector< Bullet* > bullets_;
    std::vector< Balloon* > balloons_;

    QuadTreeNode* parent_;
    QuadTreeNode* children_[4];
};

#endif
