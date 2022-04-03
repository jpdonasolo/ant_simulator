#pragma once

#include "entity.h"


class Pheromone : public Entity {
public:
    Pheromone(int x, int y, int anthillIndex)
    : Entity(x, y)
    {}
};