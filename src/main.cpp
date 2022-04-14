#include "world.h"

int main(int argc, char const *argv[])
{
    World world;
    world.setup();
    
    bool pause = false;
	SDL_Event event;

    while(world.running)
    {
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
				world.running = false;

            if(event.type == SDL_MOUSEBUTTONUP)
            {
                pause = !pause;
            }
        }
        if(!pause){
            world.update();
            world.draw();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}