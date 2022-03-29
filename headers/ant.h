#pragma once

#include <iostream>
#include "entity.h"


class Ant : public Entity {
    public:
        Ant()
        : Entity('F') 
        {std::cout << "c\n";};

        Ant operator=(const Ant & other){
            std::cout << "operator= Ant\n";
            Ant a;
            return a;
        }
    private:
};