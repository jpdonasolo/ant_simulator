#include "pheromone.h"
#include "world.h"

void Pheromone::update(std::list<Pheromone>::iterator & it)
{
    remainingLife--;
    if (remainingLife == 0)
    {   
        int tileIdx = worldP->getEntityIndex(getx(), gety(), worldP->m_chart);
        worldP->m_chart[tileIdx].pheroList[getIndex()] --;
        it = worldP->m_pheromones.erase(it);
    } else {
        std::advance(it, 1);
    }
}
