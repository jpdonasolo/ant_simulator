#pragma once

#include <vector>


class Tile {
public:
    Tile(int sizePheroList) 
        { pheroList.resize(sizePheroList, 0); };

    std::vector<int> pheroList; 
	bool isFood = false;
	bool isAnthill = false;
private:
};