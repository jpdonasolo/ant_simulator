#pragma once

#include <iostream>
#include "object.h"


class Ant : public Object {
    public:
        Ant()
        : Object('F') 
        {std::cout << "c\n";};

        Ant operator=(const Ant & other){
            std::cout << "operator= Ant\n";
            Ant a;
            return a;
        }
    private:
};