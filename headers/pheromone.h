#pragma once

#include <list>
#include <string>

#include "entity.h"
#include "utils.h"

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
    std::string getColor(){ return color(getIndex()%TOTALCOLORS); }
    void update(std::list<Pheromone>::iterator&);
    
    int remainingLife;
private:
    const int baseLifetime;
    const int indAnthill;
    World * worldP;
};
