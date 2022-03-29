#pragma once

#include <iostream>
#include <vector>

#include "ant.h"
#include "entity.h"

class World{
public:
    
    World(int width, int height) 
    : width(width), height(height) {};
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

World::World(int width, int height)
    : width(width), height(height)
{
    chart.resize(width * height);
    Entity * emptySpace =  new Entity();

    for (int i = 0; i < chart.size(); i++){
        chart[i] = emptySpace;
    }
}