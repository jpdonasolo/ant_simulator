#pragma once

#include "entity.h"


class Anthill : public Entity {
public:
    Anthill(int x, int y, int anthillIndex, int nAnts)
        : Entity(x, y)
        , ind(anthillIndex)
        , popu(nAnts)
        {}

    int getIndex() const { return ind; };
    int getPopu() const { return popu; };

private:
    const int ind;
    int popu;
};