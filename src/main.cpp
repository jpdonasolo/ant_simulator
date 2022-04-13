#include "world.h"

int main(int argc, char const *argv[])
{
    
    World world;
    world.setup();
    
    bool running = true;
	SDL_Event event;

    while(running)
    {
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
				running = false;

            if(event.type == SDL_MOUSEBUTTONUP)
            {
                std::cout << "Pause\n";
            }
        }
        world.update();
        world.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}