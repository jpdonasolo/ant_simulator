#include "pheromone.h"
#include "world.h"

void Pheromone::update()
{
    remainingLife--;
    if(remainingLife==0)
    {
        int tileIdx = worldP->getEntityIndex(getx(), gety(), worldP->m_chart);
        worldP->m_chart[tileIdx]->pheroList[getIndex()]--;
        toRemove = true;
    }
}
