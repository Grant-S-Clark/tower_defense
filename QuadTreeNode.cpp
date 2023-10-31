//Name: Grant Clark
//File: QuadTreeNode.cpp

#include "QuadTreeNode.h"

// -1 max depth means no maximum depth, or rather,
// it can recur down infinitely.
void QuadTreeNode::distribute(const int threshold, const int max_depth)
{
    if (balloons_.size() + bullets_.size() > threshold &&
        (depth_ < max_depth || max_depth == -1))
    {
        has_children_ = true;
        
        /// Create new children for the quad tree to distribute
        /// Objects into.
        
        //Quadrant 1
        children_[0] = new QuadTreeNode(depth_ + 1,                    // New depth
                                        rect_.left + rect_.width / 2,  // New left
                                        rect_.top,                     // New top
                                        rect_.width / 2,               // New width
                                        rect_.height / 2,              // New height
                                        this);                         // New parent
        //Quadrant 2
        children_[1] = new QuadTreeNode(depth_ + 1,
                                        rect_.left,
                                        rect_.top,
                                        rect_.width / 2,
                                        rect_.height / 2,
                                        this);
        //Quadrant 3
        children_[2] = new QuadTreeNode(depth_ + 1,
                                        rect_.left,
                                        rect_.top + rect_.height / 2,
                                        rect_.width / 2,
                                        rect_.height / 2,
                                        this);
        //Quadrant 4
        children_[3] = new QuadTreeNode(depth_ + 1,
                                        rect_.left + rect_.width / 2,
                                        rect_.top + rect_.height / 2,
                                        rect_.width / 2,
                                        rect_.height / 2,
                                        this);

        //Distribute the bullets
        std::vector< std::vector< Bullet* > > bullet_dist =
            {{}, {}, {}, {}}; //4 empty vectors.
        int count, distribution_num;
        for (int i = 0; i < bullets_.size(); ++i)
        {
            count = 0;
            if (children_[0]->rect_.intersects(bullets_.at(i)->getGlobalBounds()))
            {
                distribution_num = 0;
                count += 1;
            }

            if (children_[1]->rect_.intersects(bullets_.at(i)->getGlobalBounds()))
            {
                distribution_num = 1;
                count += 1;
            }

            if (count < 2 &&
                children_[2]->rect_.intersects(bullets_.at(i)->getGlobalBounds()))
            {
                distribution_num = 2;
                count += 1;
            }
            
            if (count < 2 &&
                children_[3]->rect_.intersects(bullets_.at(i)->getGlobalBounds()))
            {
                distribution_num = 3;
                count += 1;
            }

            if (count == 1)
            {
                bullet_dist.at(distribution_num).push_back(bullets_.at(i));
                bullets_.erase(bullets_.begin() + i);
                --i;
            }
        }

        //Give bullets to children.
        for (int i = 0; i < 4; ++i)
            children_[i]->bullets_ = bullet_dist.at(i);

        //Distribute the balloons
        std::vector< std::vector< Balloon* > > balloon_dist =
            {{}, {}, {}, {}}; //4 empty vectors.
        for (int i = 0; i < balloons_.size(); ++i)
        {
            count = 0;
            if (children_[0]->rect_.intersects(balloons_.at(i)->getGlobalBounds()))
            {
                distribution_num = 0;
                count += 1;
            }

            if (children_[1]->rect_.intersects(balloons_.at(i)->getGlobalBounds()))
            {
                distribution_num = 1;
                count += 1;
            }

            if (count < 2 &&
                children_[2]->rect_.intersects(balloons_.at(i)->getGlobalBounds()))
            {
                distribution_num = 2;
                count += 1;
            }
            
            if (count < 2 &&
                children_[3]->rect_.intersects(balloons_.at(i)->getGlobalBounds()))
            {
                distribution_num = 3;
                count += 1;
            }

            if (count == 1)
            {
                balloon_dist.at(distribution_num).push_back(balloons_.at(i));
                balloons_.erase(balloons_.begin() + i);
                --i;
            }
        }

        //Give balloons to children.
        for (int i = 0; i < 4; ++i)
            children_[i]->balloons_ = balloon_dist.at(i);
                

        //Recur down children and check if they need distribution.
        for (int i = 0; i < 4; ++i)
            children_[i]->distribute(threshold, max_depth);
    }

    return;
}

#include <iostream>
void QuadTreeNode::get_collisions(std::vector< std::vector< sf::CircleShape* > > & collisions) const
{
    // Each sub-vector of the collisions vector
    // holds 2 sf::CircleShape objects,
    // the first will be a bullet, and the second
    // will be the balloon it collides with.

    int bullet_size = bullets_.size();
    int balloon_size = balloons_.size();

    //Bullets and Balloons on this leaf.
    for (int i = 0; i < bullet_size; ++i)
    {
        for (int j = 0; j < balloon_size; ++j)
        {
            if (circle_collision(*bullets_.at(i), *balloons_.at(j)))
            {
                collisions.push_back(std::vector< sf::CircleShape* >(2));
                collisions.back().at(0) = bullets_.at(i);
                collisions.back().at(1) = balloons_.at(j);
            }
        }
    }

    //Bullets on this leaf and Balloons on parent leaves.
    QuadTreeNode * p = parent_;
    while (p != nullptr)
    {
        for (int i = 0; i < bullet_size; ++i)
        {
            for (int j = 0, n = p->balloons_.size(); j < n; ++j)
            {
                if (circle_collision(*bullets_.at(i), *p->balloons_.at(j)))
                {
                    collisions.push_back(std::vector< sf::CircleShape* >(2));
                    collisions.back().at(0) = bullets_.at(i);
                    collisions.back().at(1) = p->balloons_.at(j);
                }
            }
        }

        p = p->parent_;
    }

    //Bullets on parent leaves and Balloons on this leaf.
    p = parent_;
    while (p != nullptr)
    {
        for (int i = 0; i < balloon_size; ++i)
        {
            for (int j = 0, n = p->bullets_.size(); j < n; ++j)
            {
                if (circle_collision(*p->bullets_.at(j), *balloons_.at(i)))
                {
                    collisions.push_back(std::vector< sf::CircleShape* >(2));
                    collisions.back().at(0) = p->bullets_.at(j);
                    collisions.back().at(1) = balloons_.at(i);
                }
            }
        }

        p = p->parent_;
    }

    //Recur through children if applicable.
    if (has_children_)
    {
        for (int i = 0; i < 4; ++i)
            children_[i]->get_collisions(collisions);
    }

    //Return with completed vector of collided objects
    //stored in parameter vector.
    return;
}
