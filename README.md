# tower_defense
A simple tower defense game programmed in C++ computing collisions using a quadtree.

To build the program into an executable, you need to first install SFML. Next, you will need
to navigate to the makefile, and replace all instances of "-I/usr/local/SFML/include" and
"-L/usr/local/SFML/lib" with the path to your specific SFML installation. Then you can
execute "make e" to build the program, and then "make r" to run it.

To play the game, simply drag and drop the turrets onto the screen from the right sidebar
where they are located and place them in valid spots. You can right-click on a turret to
remove it. This game does not provide much gameplay but is more focused on being a fun project
I did when studying quadtrees.
