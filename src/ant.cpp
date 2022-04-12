#include <iostream>

#include "ant.h"
#include "world.h"
#include "food.h"

#include <vector>


char Ant::getMarker()
{
    if (mode == seek) {
        return 'A'; 
    } else {
        return 'B'; 
    }
}


void Ant::lookTo(int x, int y)
{
    if (x > posx)
    {
        face = east;
    } else if (x < posx)
    {
        face = west;
    } else if (y > posy)
    {
        face = south;
    } else if (y < posy)
    {
        face = north;
    }
}


void Ant::leavePhero()
{
    Pheromone * phero = new Pheromone(getx(), gety(), getAnthillIndex(), worldP->config["pheroLifetime"].asInt(), worldP);
    Tile * pheroTile = worldP->m_chart[worldP->posToInt(getx(), gety())];
    pheroTile->pheroList[getAnthillIndex()]++;
    worldP->m_pheromones.push_back(phero);
}


/* Checar se tá num tile de food
 * Checar se tem comida
 * Requisitar bastões
 * Se conseguir:
 *  checar se ainda tem comida
 *  comer
 * Se não conseguir:
 *  ficar de guarda (por enquanto = fazer nada)
 */
void Ant::checkFood()
{
    Tile * antTile = worldP->m_chart[worldP->posToInt(getx(), gety())];
    if (antTile->isFood == true)
    {
        int foodIndex = worldP->getEntityIndex(getx(), gety(), worldP->m_foods);
        Food * food = worldP->m_foods[foodIndex];
        if (food->currentFood > 0 && mode == seek)
        {
            if (hasSeat(food))
            {
                int seat = takeSeat(food);
                // Philosophers dinner
            } else {
                takeGuard();
            }
        }
    }
}

int Ant::takeSeat(Food * food)
{
    for (int i = 0; i < food->numSeats; i++)
    {
        if (food->seats[i].try_lock())
        {
            return i;
        }
    }
    return -1; // This should NOT happen
}


void Ant::checkAnthill()
{
    Tile * antTile = worldP->m_chart[worldP->posToInt(getx(), gety())];
    if(antTile->isAnthill == true)
    {
        int anthillIndex = worldP->getEntityIndex(getx(), gety(), worldP->m_anthills);
        if(anthillIndex == getAnthillIndex() && mode == bring)
        {
            mode = seek;
        }
    }
}


bool Ant::checkInvalidCoordinates(int posx, int posy)
{   
    if(posx >= worldP->getWidth() || posx < 0 || posy >= worldP->getHeight() || posy < 0){ return true; }
    return false;
}


void Ant::look()
{
    // Obtém tile da formiga
    Tile * antTile = worldP->m_chart[worldP->posToInt(getx(), gety())];
    
    // Obtém tiles que a formiga enxerga (quadrado 2*vision X vision na
    // frente dela)
    const int vision = worldP->config["vision"].asInt();
    std::list<std::vector<int>> pointsToCheck;
    switch(face){
        case north:
            for (int i = 0; i < vision; i++) {
                for (int j = -vision; j < vision+1; j++) {
                    std::vector<int> coords;
                    coords.push_back(getx()+j);
                    coords.push_back(gety()-i);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
        case east:
            for (int i = 0; i < vision; i++) {
                for (int j= -vision; j < vision+1; j++) {
                    std::vector<int> coords;
                    coords.push_back(getx()+i);
                    coords.push_back(gety()+j);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
        case south:
            for (int i = 0; i < vision; i++) {
                for (int j = -vision; j < vision+1; j++) {
                    std::vector<int> coords;
                    coords.push_back(getx()+j);
                    coords.push_back(gety()+i);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
        case west:
            for (int i = 0; i < vision; i++) {
                for (int j = -vision; j < vision+1; j++) {
                    std::vector<int> coords;
                    coords.push_back(getx()-i);
                    coords.push_back(gety()+j);
                    pointsToCheck.push_back(coords);
                }
            }
            break;
    }

    // Remove tiles inválidos (mesmo tile que a formiga, ta fora do mapa,
    // etc.)
    auto point = pointsToCheck.begin();
    while (point != pointsToCheck.end()) 
    {
        if (checkInvalidCoordinates((*point)[0], (*point)[1]) || ((*point)[0] == getx() && (*point)[1] == gety()))
        {
            point = pointsToCheck.erase(point);
        } else {
            ++point;
        }
    }

    // Antes tínhamos as coordenadas, agora vamos obter pointeiros para
    // os tiles mesmo.
    std::vector<Tile*> tilesVision;
    for(auto it = pointsToCheck.begin(); it != pointsToCheck.end(); it++)
    {       
        tilesVision.push_back(worldP->m_chart[worldP->posToInt((*it)[0],(*it)[1])]);
    }

    // Vamos ver qual tile tem mais feromônios da colônia daquela formiga
    int maxPhero = 0;
    Tile * tileObj;

    for (Tile * tileP : tilesVision)
    {
        
        if ((tileP->pheroList)[getAnthillIndex()] > maxPhero)
        {   
            maxPhero = (tileP->pheroList)[getAnthillIndex()];
            tileObj = tileP;
        }
    }

    // checa se tá bring, direciona formigueiro
    if(mode == bring)
    {
       lookTo(worldP->m_anthills[getAnthillIndex()]->getx(), worldP->m_anthills[getAnthillIndex()]->gety());
    } else if(maxPhero > 0)
    {
    // caso contrario, checa por tiles com feromonio de comida aos lados, exceto no seu tile
        lookTo(tileObj->getx(), tileObj->gety());
    } else if ((antTile->pheroList)[getAnthillIndex()] > 0)
    {
    // caso haja nenhum, checa se seu tile esta com feromonio (virar direção)
        face = mapInd((mapDir(face)+2)%4);
    } else
    {
    // caso nenhuma das condicoes anteriores, ande para frente com prob 6, 3 pros lados e 1 para trás
        int weight[4];
        weight[mapDir(face)] = 10;
        weight[(mapDir(face)+1)%4] = 2;
        weight[(mapDir(face)-1)%4] = 2;
        weight[(mapDir(face)+2)%4] = 1;
        face = randDir(weight);
    }
}


void Ant::walk()
{   
    int uniformWeightN[4] = {0, 1, 1, 1};
    int uniformWeightE[4] = {1, 0, 1, 1};
    int uniformWeightS[4] = {1, 1, 0, 1};
    int uniformWeightW[4] = {1, 1, 1, 0};
    switch(face)
    {
        case north:
            if (gety()==0)
            {
                face = randDir(uniformWeightN);
            } else
            {
                suby();
            }
            break;
        case east:
            if (getx() == worldP->getWidth() - 1)
            {
                face = randDir(uniformWeightE);
            } else
            {
                addx();
            }
            break;
        case south:
            if (gety() == worldP->getHeight() - 1)
            {
                face = randDir(uniformWeightS);
            } else
            {
                addy();
            }
            break;
        case west:
            if (getx() == 0)
            {
                face = randDir(uniformWeightW);
            } else
            {
                subx();
            }
            break;
        default:
            break;
    }
}


void Ant::update()
{
    checkFood();
    checkAnthill();
    if (mode == bring){ leavePhero(); }
    look();
    walk();
}
