#include "world.h"

int main(int argc, char const *argv[])
{
    
    World world;
    world.setup();
    world.print();
    world.update();

    return 0;
}
