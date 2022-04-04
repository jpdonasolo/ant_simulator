#pragma once

#include "entity.h"


class Ant : public Entity {
public:
    Ant(int x, int y, int anthillIndex)
        : Entity(x, y)
        , indAnthill(anthillIndex)
        {}

    int getindex() const { return indAnthill; }
    
    char getMarker() { return 'A'; }

private:
    const int indAnthill;
};