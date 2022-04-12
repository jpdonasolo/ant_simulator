#include "world.h"

int main(int argc, char const *argv[])
{
    
    World world;
    world.setup();

    while (true)
    {
        world.update();
        world.print();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}