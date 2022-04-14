#pragma once

#include <string>
#include <mutex>

#include "entity.h"
#include "utils.h"


class Anthill : public Entity {
public:
    Anthill(int x, int y, int anthillIndex, int nAnts)
        : Entity(x, y)
        , ind(anthillIndex)
        , popu(nAnts)
        {}

    int getIndex() const { return ind; };
    std::string getColor(){ return color(getIndex()%TOTALCOLORS); }
    int getPopu() const { return popu; };

    char getMarker() { return 'H'; }
    int storedFood = 0;
    std::mutex storedFoodCounterMutex;

private:
    const int ind;
    int popu;
};