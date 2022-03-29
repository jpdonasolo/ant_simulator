#pragma once

#include <iostream>

class Object{
    public:
        Object() : marker(' ') {std::cout << "a\n";};
        Object(char m) : marker(m) {std::cout << "b\n";};
        
        Object operator=(const Object & other){
            std::cout << "operator= Object\n";
            Object obj(other.getMarker());
            return obj;
        }

        char getMarker() const {
            return marker;
        }

    protected:
        const char marker;
};