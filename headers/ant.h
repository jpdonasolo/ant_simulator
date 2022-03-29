#pragma once

#include <iostream>
#include "entity.h"


class Ant : public Entity {
    public:
        Ant() : Entity('F') {};

        bool isEmpty() { return false; }
};