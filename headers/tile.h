#pragma once

#include <vector>

#include "entity.h"


class Tile : public Entity {
public:
    Tile(int x, int y, int sizePheroList)
    : Entity(x, y)
    {
        pheroList.resize(sizePheroList, 0);
    };

    std::vector<int> pheroList; 
	bool isFood = false;
	bool isAnthill = false;
private:
};