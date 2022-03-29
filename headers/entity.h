#pragma once

#include <iostream>

class Entity{
    public:
        Entity() : marker(' ') {};
        Entity(char m) : marker(m) {};
        
        char getMarker() const { return marker; }
        virtual bool isEmpty() const { return true; };

    protected:
        const char marker;
};