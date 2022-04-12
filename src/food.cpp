#include <iostream>

#include "ant.h"
#include "food.h"
#include "world.h"
#include "pheromone.h"


void Food::update()
{   
    int tileIdx = worldP->getEntityIndex(getx(), gety(), worldP->m_chart);
    currentTime--;
    if(currentTime==0)
    {
        currentTime = refillTime;
        currentFood += refillQuantity;
        worldP->m_chart[tileIdx]->hasFood = true;
    }
    
    if(currentFood==0)
    {
        worldP->m_chart[tileIdx]->hasFood = false;
    }
}
