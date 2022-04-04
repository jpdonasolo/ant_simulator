#pragma once

#include "entity.h"


class FoodSource : public Entity {
public:
    FoodSource(int x, int y, int rt, int qt)
        : Entity(x, y)
        , refillTime(rt)
        , refillQuantity(qt)
        {}

    int getRefillTime() const { return refillTime; };
    int getRefillQuantity() const { return refillQuantity; };

    char getMarker() { return 'F'; }

private:
    const int refillTime;
    const int refillQuantity;
};