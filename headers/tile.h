#include "entity.h"

#include <vector>

struct Tile {
public:
    Tile(){};

    bool isEmpty() { return m_entities.size() == 0; };
private:
    std::vector<Entity> m_entities;
};