#pragma once

#include <vector>
#include <mutex>

#include "entity.h"


class Tile : public Entity {
public:
    Tile(int x, int y, int sizePheroList) 
        : Entity(x, y)
        { pheroList.resize(sizePheroList, 0); };

    char getMarker() { return 'T'; }
    void decreasePheroLife(int pheroIdx);
    void increasePheroLife(int pheroIdx);

    std::vector<int> pheroList; 
	bool hasFood = false;
	bool isAnthill = false;
private:
    std::mutex m;
};