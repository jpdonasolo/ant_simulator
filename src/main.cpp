#include "../headers/ant.h"
#include "../headers/world.h"

int main(int argc, char const *argv[])
{
    
    World world (3, 3);
    Ant ant;

    world.addObjectToMap<Ant>(ant, 0, 0);
    
    world.print();

    return 0;
}
