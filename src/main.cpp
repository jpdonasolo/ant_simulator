#include "../headers/ant.h"
#include "../headers/world.h"

int main(int argc, char const *argv[])
{
    
    World world (3, 3);
    Ant * ant = new Ant();

    world.addAntToChart(ant, 1, 2);
    
    world.print();

    return 0;
}
