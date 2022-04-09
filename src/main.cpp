#include "world.h"

int main(int argc, char const *argv[])
{
    
    World world;
    world.setup();
    for(int i=0; i<30; i++){
        world.print();
        world.update();
    }
    world.print();

    return 0;
}