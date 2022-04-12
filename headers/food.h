#pragma once

#include "entity.h"

class World;

class Food : public Entity {
public:
    Food(int x, int y, int rt, int qt, int iq, World * worldPtr)
        : Entity(x, y)
        , refillTime(rt)
        , refillQuantity(qt)
        , currentFood(iq)
        , currentTime(rt)
        , worldP(worldPtr)
        {}

    int getRefillTime() const { return refillTime; };
    int getRefillQuantity() const { return refillQuantity; };

    char getMarker() { return 'F'; }

    void update();
    
    int currentTime;
    int currentFood;

private:
    const int refillTime;
    const int refillQuantity;
    World * worldP;
};