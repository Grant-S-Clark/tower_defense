//Name: Grant Clark
//File: main.cpp

#include <iostream>

#include "Global.h"
#include "Balloon.h"
#include "BasicTurret.h"
#include "QuadTree.h"
#include "SpawnRect.h"

Balloon * get_leader(const std::vector< Balloon* > &,
                     const Path &);
void collisions(QuadTree &,
                std::vector< Bullet* > &,
                std::vector< Balloon* > &);
int get_selected_turret(const std::vector< Turret* > &,
                        const sf::Vector2i &);

int main()
{
    std::srand((unsigned int)time(NULL));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Tower Defense");
    window.setFramerateLimit(60);
    window.setActive(true);
    
    bool in_focus = true, space_pressed = false, r_clicking = false;
    int turret_index = -1;
    
    //Draw this before calling opengl drawing functions.
    sf::CircleShape gl_helper(0);

    Path::set_window(&window);
    Path path(0, WIDTH - MENU_WIDTH, 0, HEIGHT);
    path.generate(5); //Scale factor of 5
    
    //Give balloons the path.
    Balloon::set_path(&path);
    std::vector< Balloon* > balloons;
    int frame_count = 0;
    int spawn_rate = 30; //Frames per spawn.
    int timer = 0; //Timer for increasing difficulty.
    
    Turret::set_window(&window);
    std::vector< Turret* > turrets;

    std::vector< Bullet* > bullets;

    QuadTree quad_tree(5, -1,
                       0, 0, WIDTH, HEIGHT,
                       &bullets, &balloons);

    sf::RectangleShape menu_background(sf::Vector2f(MENU_WIDTH, HEIGHT));
    menu_background.setPosition(WIDTH - MENU_WIDTH, 0);
    menu_background.setFillColor(sf::Color::Black);
    menu_background.setOutlineThickness(5);
    menu_background.setOutlineColor(sf::Color(125, 125, 125));

    SpawnRect::set_window(&window);
    SpawnRect::set_turrets(&turrets);
    SpawnRect::set_path(&path);
    std::vector< SpawnRect* > spawn_rects;
    spawn_rects.push_back(
        new SpawnRect(40, 40,
                      WIDTH - MENU_WIDTH + (MENU_WIDTH / 2),
                      HEIGHT / 2,
                      Basic) //Basic turret spawner.
        );
    spawn_rects.push_back(
        new SpawnRect(40, 40,
                      WIDTH - MENU_WIDTH + (MENU_WIDTH / 2),
                      HEIGHT / 2 + 50,
                      Sniper)
        );
    
    
    /*
      NOTE: I am not using the window.isOpen() loop to sustain the while loop,
      as when you use openGL functions there is extra cleanup that must be
      done in the loop before the program terminates that cannot be done
      if the window was closed beforehand.
    */
    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                in_focus && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                running = false;
            }

            //Adjust window focus
            if (in_focus && event.type == sf::Event::LostFocus)
                in_focus = false;
            else if (!in_focus && event.type == sf::Event::GainedFocus)
                in_focus = true;

            //Adjust window if resized.
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        if (in_focus)
        {
            //Spacebar checks.
            if (!space_pressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                space_pressed = true;

                timer = 0;
                frame_count = 0;
                spawn_rate = 30;
                
                path.generate(5);
            
                for (int i = 0, n = balloons.size(); i < n; ++i)
                    delete balloons.at(i);
                balloons.clear();

                for (int i = 0, n = turrets.size(); i < n; ++i)
                    delete turrets.at(i);
                turrets.clear();

                for (int i = 0, n = bullets.size(); i < n; ++i)
                    delete bullets.at(i);
                bullets.clear();
            }
            else if (space_pressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                space_pressed = false;

            //Make balloons
            if (frame_count >= spawn_rate ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
                balloons.push_back(new Balloon(rand() % 41 + 10));
                frame_count = 0;
            }
            else
                ++frame_count;

            //Adjust difficulty with timer.
            if (timer >= 3600) //One minute.
            {
                timer = 0;
                spawn_rate -= 10;
                if (spawn_rate < 0)
                    spawn_rate = 0;
            }
            else
                ++timer;
            
            
        
            //Adjust Balloons.
            for (int i = 0; i < balloons.size(); ++i)
            {
                balloons.at(i)->follow_path();
                if (balloons.at(i)->end_of_path())
                {
                    delete balloons.at(i);
                    balloons.erase(balloons.begin() + i);
                    --i;
                }
            }

            //Adjust bullets.
            for (int i = 0, n = bullets.size(); i < n; ++i)
                bullets.at(i)->move();

            //Turret aiming and shooting.
            for (Turret * & t : turrets)
            {
                Balloon * target = get_leader(
                    t->balloons_in_range(balloons), path
                    );
            
                //If leader balloon exists, aim at it and fire.
                if (target != nullptr)
                {
                    t->aim_at(*target);
                    std::vector < Bullet* > new_bullets = t->fire();

                    if (!new_bullets.empty())
                    {
                        bullets.insert(bullets.end(),
                                       new_bullets.begin(),
                                       new_bullets.end());
                    }
                }
            }

            //Checking Collisions
            collisions(quad_tree, bullets, balloons);

            //Deleting of off screen bullets.
            for (int i = 0; i < bullets.size(); ++i)
            {
                if (off_screen(*bullets.at(i)))
                {
                    delete bullets.at(i);
                    bullets.erase(bullets.begin() + i);
                    --i;
                }
            }

            //Turret Spawning
            for (SpawnRect * & s : spawn_rects)
            {
                s->update();
                Turret * new_turret = s->get_new_turret();
                if (new_turret != nullptr)
                    turrets.push_back(new_turret);
            }

            //Turret deleting
            if (!r_clicking && sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                r_clicking = true;
                turret_index = get_selected_turret(turrets,
                                                   sf::Mouse::getPosition(window));
            }

            if (r_clicking && !sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                r_clicking = false;

                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                if (turret_index != -1 &&
                    turrets.at(turret_index)->body().getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
                {
                    delete turrets.at(turret_index);
                    turrets.erase(turrets.begin() + turret_index);
                }
            }

            ///Drawing
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Draw turret range seperately so it does not overlap
            //on top of other objects except for the path. Only draw
            //a turret's range if it is being hovered over by the mouse.
            {
                int i = turret_index = get_selected_turret(turrets,
                                                           sf::Mouse::getPosition(window));
                if (i != -1)
                    turrets.at(i)->draw_range();
            }
            
            //OpenGL path drawing.
            path.draw();

            //SFML balloon drawing.
            for (Balloon * & b : balloons)
                window.draw(*b);

            //SFML bullet drawing
            for (Bullet * & b : bullets)
                window.draw(*b);

            //SFML turret drawing
            for (Turret * & t : turrets)
                t->draw();

            //Menu assets.
            window.draw(menu_background);
            for (SpawnRect * & s : spawn_rects)
                s->draw();

            window.display();
        } //in_focus
    }

    window.close();

    // free memory.
    for (int i = 0, n = balloons.size(); i < n; ++i)
        delete balloons.at(i);
    for (int i = 0, n = turrets.size(); i < n; ++i)
        delete turrets.at(i);
    for (int i = 0, n = bullets.size(); i < n; ++i)
        delete bullets.at(i);
    for (int i = 0, n = spawn_rects.size(); i < n; ++i)
        delete spawn_rects.at(i);
    
    return 0;
}


Balloon * get_leader(const std::vector< Balloon* > & b,
                     const Path & path)
{
    if (b.size() == 0)
        return nullptr;
    
    std::vector< int > indexes;
    int path_max = -1;

    for (int i = 0, n = b.size(); i < n; ++i)
    {
        if (b.at(i)->path_index() > path_max)
        {
            indexes.clear();
            path_max = b.at(i)->path_index();
        }

        if (b.at(i)->path_index() == path_max)
            indexes.push_back(i);
    }
    
    Direction dir = path[path_max].dir;
    float max, t;
    int leader;

    //Set initial values.
    switch (dir)
    {
        case Forward:
            max = b.at(indexes[0])->getPosition().x;
            break;
        case Up:
        case Down:
            max = b.at(indexes[0])->getPosition().y;
            break;
    }
    leader = indexes[0];
    
    for (int i = 1, n = indexes.size(); i < n; ++i)
    {
        switch (dir)
        {
            case Forward:
                t = b.at(indexes[i])->getPosition().x;
                if (t > max)
                {
                    max = t;
                    leader = indexes[i];
                }
                break;
            case Up:
                t = b.at(indexes[i])->getPosition().y;
                if (t < max)
                {
                    max = t;
                    leader = indexes[i];
                }
                break;
            case Down:
                t = b.at(indexes[i])->getPosition().y;
                if (t > max)
                {
                    max = t;
                    leader = indexes[i];
                }
                break;
        }
    }

    return b.at(leader);
}


void collisions(QuadTree & quad_tree,
                std::vector< Bullet* > & bullets,
                std::vector< Balloon* > & balloons)
{
    std::vector< std::vector< sf::CircleShape* > > collisions;

    //Update quad tree and retrieve list of collisions.
    quad_tree.update();
    collisions = quad_tree.get_collisions();

    std::unordered_set< sf::CircleShape* > killed_objects;
    killed_objects.reserve((bullets.size() + balloons.size()) * 1.5);
    
    for (int i = 0, n = collisions.size(); i < n; ++i)
    {
        if (
            killed_objects.find(collisions.at(i).at(0)) ==
            killed_objects.end() &&
            killed_objects.find(collisions.at(i).at(1)) ==
            killed_objects.end()
            )
        {
            //insert into the set of killed objects.
            killed_objects.insert(collisions.at(i).at(0));
            killed_objects.insert(collisions.at(i).at(1));
        }
    }

    ///Loop through objects and if it was killed, do something.
    //Bullets
    for (int i = 0; i < bullets.size(); ++i)
    {
        if (killed_objects.find(bullets.at(i)) !=
            killed_objects.end())
        {
            delete bullets.at(i);
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }
    //Balloons
    for (int i = 0; i < balloons.size(); ++i)
    {
        if (killed_objects.find(balloons.at(i)) !=
            killed_objects.end())
        {
            delete balloons.at(i);
            balloons.erase(balloons.begin() + i);
            --i;
        }
    }

    return;
}


int get_selected_turret(const std::vector< Turret* > & turrets,
                        const sf::Vector2i & mouse_pos)
{
    for (int i = 0, n = turrets.size(); i < n; ++i)
        if (turrets.at(i)->body().getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
            return i;
    
    return -1;
}
