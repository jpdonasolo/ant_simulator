#pragma once

#include "entity.h"


class Pheromone : public Entity {
public:
    Pheromone(int x, int y, int anthillIndex, int lifetime)
    : Entity(x, y)
    , indAnthill(anthillIndex)
    , baseLifetime(lifetime)
    , remainingLife(lifetime){}

    char getMarker() { return 'P'; }
    int getIndex() { return indAnthill; }
    
    int remainingLife;
private:
    const int baseLifetime;
    const int indAnthill;
};