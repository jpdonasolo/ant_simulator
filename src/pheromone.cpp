#include "pheromone.h"
#include "world.h"

void Pheromone::update(std::list<Pheromone*>::iterator & it)
{
    remainingLife--;
    if (remainingLife == 0)
    {
        it = worldP->m_pheromones.erase(it);
    } else {
        std::advance(it, 1);
    }
}
