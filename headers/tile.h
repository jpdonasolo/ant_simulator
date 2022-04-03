#pragma once

#include <vector>

#include "entity.h"


struct Tile : public Entity {
public:
    Tile(int x, int y)
    : Entity(x, y)
    {};
private:
};