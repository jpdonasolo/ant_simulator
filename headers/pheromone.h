#pragma once

#include "entity.h"
#include <list>

class World;

class Pheromone : public Entity {
public:
    Pheromone(int x, int y, int anthillIndex, int lifetime, World * worldPtr)
    : Entity(x, y)
    , indAnthill(anthillIndex)
    , baseLifetime(lifetime)
    , remainingLife(lifetime)
    , worldP(worldPtr){}

    char getMarker() 
    { 
        return '.';
    }
    int getIndex() { return indAnthill; }
    void update(std::list<Pheromone*>::iterator&);
    
    int remainingLife;
private:
    const int baseLifetime;
    const int indAnthill;
    World * worldP;
};
