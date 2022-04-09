#pragma once

#include "entity.h"


class Food : public Entity {
public:
    Food(int x, int y, int rt, int qt)
        : Entity(x, y)
        , refillTime(rt)
        , refillQuantity(qt)
        , currentFood(qt)
        , currentTime(rt)
        {}

    int getRefillTime() const { return refillTime; };
    int getRefillQuantity() const { return refillQuantity; };

    char getMarker() { return 'F'; }
    
    int currentTime;
    int currentFood;

private:
    const int refillTime;
    const int refillQuantity;
};