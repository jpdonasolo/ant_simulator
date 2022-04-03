#pragma once

#include "entity.h"


class Ant : public Entity {
public:
    Ant(int x, int y, int anthillIndex)
        : Entity(x, y)
        , indAnthill(anthillIndex)
        {}

    int getindex() const { return indAnthill; };

private:
    const int indAnthill;
};