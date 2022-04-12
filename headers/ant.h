#pragma once

#include "entity.h"
#include "food.h"
#include "semaphore.h"
#include <mutex>

// Para poder ter um ponteiro para o world
class World;
enum antBehavior { seek, bring };
enum dir { north=0, east=1, south=2, west=3};

class Ant : public Entity {
public:
    Ant(int x, int y, int anthillIndex, World * worldPtr)
        : Entity(x, y)
        , indAnthill(anthillIndex)
        , worldP(worldPtr)
        {}

    int getAnthillIndex() const { return indAnthill; }
    char getMarker();
    void lookTo(int x, int y);
    void leavePhero();

    // Food related stuff
    void checkFood();
    bool hasSeat(Food * food) { return food->seatsAvailable.try_acquire(); }
    int takeSeat(Food * food);
    bool requestSticks(Food * food);
    bool requestStick(Food * food);
    void eat(Food * food);
    void takeGuard() {};

    void checkAnthill();
    bool checkInvalidCoordinates(int posx, int posy);
    void look();
    void walk();
    void update();

    dir face;
    antBehavior mode = seek;
private:
    const int indAnthill;
    World * worldP;
};
