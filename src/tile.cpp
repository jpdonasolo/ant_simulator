#include "tile.h"

void Tile::decreasePheroLife(int pheroIdx)
{
    std::lock_guard<std::mutex> lg(m);
    pheroList[pheroIdx]--;
}

void Tile::increasePheroLife(int pheroIdx)
{
    std::lock_guard<std::mutex> lg(m);
    pheroList[pheroIdx]--;
}