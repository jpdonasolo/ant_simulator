#include "ant.h"
#include "world.h"

char Ant::getMarker()
{
    if(mode == seek){
        return 'A'; 
    }else{
        return 'B'; 
    }
}

void Ant::lookTo(int x, int y)
{
    if(x > posx)
    {
        face = east;
    }else if(x < posx)
    {
        face = west;
    }else if(y > posy)
    {
        face = south;
    }else if(y < posy)
    {
        face = north;
    }
}

void Ant::update()
{
    /* Requer acesso a:
     *  m_chart
     *  config["vision"]
     *
     */
    worldP->m_chart;
}

void Ant::leavePhero()
{
    Pheromone * phero = new Pheromone(getx(), gety(), getAnthillIndex(), worldP->config["pheroLifetime"].asInt());
    Tile * pheroTile = &(worldP->m_chart[worldP->posToInt(getx(), gety())]);
    pheroTile->pheroList[getAnthillIndex()] ++;
    worldP->m_pheromones.push_back(*(phero));
}
