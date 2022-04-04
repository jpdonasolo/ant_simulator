#pragma once

#include "entity.h"


class Ant : public Entity {
public:
    Ant(int x, int y, int anthillIndex)
        : Entity(x, y)
        , indAnthill(anthillIndex)
        {}

    int getindex() const { return indAnthill; };

    void nextTick(){
        //world::leave_phero(this);
    }
private:
    const int indAnthill;
};