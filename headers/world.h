#pragma once

#include <iostream>
#include <vector>

#include "ant.h"
#include "entity.h"

class World{
public:
    World(int w, int h)
    : width(w), height(h)
    { chart.resize(width * height); };
    ~World(){};

    void print();
    int posToInt(int posx, int posy){
        return posx + width * posy;
    } 

private:
    std::vector<Entity *> chart;
    const int width, height;
};

void World::print()
{
    for (int pos = 0; pos < chart.size(); pos++){
        
        std::cout << chart[pos]->getMarker();
        
        if ((pos + 1 ) % width == 0){
            std::cout << std::endl;
        };
    }
}