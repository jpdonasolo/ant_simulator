#pragma once

#include "entity.h"

// Para poder ter um ponteiro para o world
class World;
enum antBehavior { seek, bring };
enum dir { north=0, east=1, south=2, west=3};

class Ant : public Entity {
public:
    Ant(int x, int y, int anthillIndex)
        : Entity(x, y)
        , indAnthill(anthillIndex)
        {}

    int getAnthillIndex() const { return indAnthill; }
    char getMarker();
    void lookTo(int x, int y);
    void leavePhero();
    void update();

    dir face;
    antBehavior mode = seek;
private:
    const int indAnthill;
    World * worldP;
};
