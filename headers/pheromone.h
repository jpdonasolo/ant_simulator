#pragma once

#include "entity.h"


class Pheromone : public Entity {
public:
    Pheromone(int x, int y, int anthillIndex, int lifetime)
    : Entity(x, y)
    , indAnthill(anthillIndex)
    , remainingLife(lifetime){}

    void nextTick(){
        --remainingLife;
    }

private:
    int remainingLife;
    const int indAnthill;
};