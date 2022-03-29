#pragma once

#include <iostream>
#include <vector>

#include "ant.h"
#include "entity.h"

class World{
    public:
        World(int w, int h)
        : width(w), height(h)
        {
                map.resize(width * height);
            };
        ~World(){};

        void nextStep(){};
        bool hasFinished(){return false;};
        void print();

        template <typename T>
        void addObjectToMap(const T &, const int, const int);

        int posToInt(int posx, int posy){
            return posx + width * posy;
        } 

    private:
        std::vector<Entity> map;
        const int width, height;
};

void World::print()
{
    for (int pos = 0; pos < map.size(); pos++){
        
        std::cout << map[pos].getMarker();
        
        if ((pos + 1 ) % width == 0){
            std::cout << std::endl;
        };
    }
}

template <typename T>
void World::addObjectToMap(const T & obj, const int posx, const int posy)
{
    const int posInMap = posToInt(posx, posy);
    map[posInMap] = obj;
}