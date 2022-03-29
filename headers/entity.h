#pragma once

#include <iostream>

class Entity{
    public:
        Entity() : marker(' ') {};
        Entity(char m) : marker(m) {};
        
        Entity operator=(const Entity & other){
            std::cout << "operator= Entity\n";
            Entity obj(other.getMarker());
            return obj;
        }

        char getMarker() const {
            return marker;
        }

    protected:
        const char marker;
};