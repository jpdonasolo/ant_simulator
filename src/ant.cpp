#include <iostream>

#include "ant.h"
#include "world.h"
#include "food.h"
#include "pheromone.h"


// Philosophers dinner stuff
const int NOT_EATING = 0;
const int HUNGRY = 1;
const int EATING = 2;

int left(int i, int N)
{
    return (i + N - 1) % N;
}

int right(int i, int N)
{
    return (i + 1) % N;
}


char Ant::getMarker()
{
    std::vector<char> dirChars = {'^','>','v','<'};
    return dirChars[mapDir(face)];
}

//enum dir { north=0, east=1, south=2, west=3};
void Ant::lookTo(int x, int y)
{   
    int weight[4] = {0,0,0,0};
    if (y < posy)
    {
        weight[0] = 1;
    }
    else if (x > posx)
    {
        weight[1] = 1;
    }
    else if (y > posy)
    {
        weight[2] = 1;
    }
    else if (x < posx)
    {
        weight[3] = 1;
    }
    face = randDir(weight);
}


void Ant::leavePhero()
{
    Pheromone * phero = new Pheromone(getx(), gety(), getAnthillIndex(), worldP->config["pheroLifetime"].asInt(), worldP);
    Tile * pheroTile = worldP->m_chart[worldP->posToInt(getx(), gety())];
    pheroTile->increasePheroLife(getAnthillIndex());
    worldP->addPheromone(phero);
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
    if (antTile->hasFood == true)
    {
        int foodIndex = worldP->getEntityIndex(getx(), gety(), worldP->m_foods);
        Food * food = worldP->m_foods[foodIndex];
        if (food->currentFood > 0 && mode == seek)
        {
            getFood(food);
        }
    }
}

void Ant::getFood(Food * food)
{
    if (hasSeat(food))
    {
        int seat = takeSeat(food);
        // Philosophers dinner
        philosopher(seat, food);
        food->seatsAvailable.release();
        food->seats[seat].unlock();
    } else {
        takeGuard();
    }
}

bool Ant::hasSeat(Food * food)
{
    return food->seatsAvailable.try_acquire();
}

int Ant::takeSeat(Food * food)
{
    // There must be a seat
    while (true)
    {
        for (int i = 0; i < food->numSeats; i++)
        {
            if (food->seats[i].try_lock())
            {
                return i;
            }
        }
    }
}

void Ant::philosopher(int i, Food * food)
{
    takeForks(i, food);
    eat(food);
    putForks(i, food);
}

void Ant::takeForks(int i, Food * food)
{
    food->m.lock();
    food->state[i] = HUNGRY;
    test(i, food);
    food->m.unlock();
    food->s[i].lock();
}

void Ant::putForks(int i, Food * food)
{
    int N = food->numSeats;
    food->m.lock();
    food->state[i] = NOT_EATING;
    test(left(i, N), food);
    test(right(i, N), food);
    food->m.unlock();
}

void Ant::test(int i, Food * food)
{
    int N = food->numSeats;
    if (food->state[i] == HUNGRY
     && food->state[left(i, N)] != EATING
     && food->state[right(i, N)] != EATING)
    {
        food->state[i] = EATING;
        food->s[i].unlock();
    }
}

void Ant::eat(Food * food)
{
    food->decreaseFoodCount();
    mode = bring;
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
    if(posx == getx() && posy == gety()){ return true; }
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
        if (checkInvalidCoordinates((*point)[0], (*point)[1]))
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
    double maxDistPhero = dist(0,0,worldP->getWidth(),worldP->getHeight());
    double maxDistFood = maxDistPhero;
    double curDist;
    bool foodInSight = false;
    Tile * tileObj = tilesVision[0];
    for (Tile * tileP : tilesVision)
    {
        curDist = dist(posx, posy, tileP->getx(), tileP->gety());
        if ((tileP->hasFood == true) && (curDist < maxDistFood))
        {
            tileObj = tileP;
            maxDistFood = curDist;
            foodInSight = true;
        }
        else if ((tileP->pheroList)[getAnthillIndex()] > 0 && foodInSight == false)
        {   
            if((tileP->pheroList)[getAnthillIndex()] >= maxPhero && curDist < maxDistPhero)
            {    
                tileObj = tileP;
                maxDistPhero = curDist;
                maxPhero = (tileP->pheroList)[getAnthillIndex()];
            }
        }
    }

    // checa se tá bring, direciona formigueiro
    if(mode == bring)
    {   
        lookTo(worldP->m_anthills[getAnthillIndex()]->getx(), worldP->m_anthills[getAnthillIndex()]->gety());
    } else if(foodInSight || maxPhero > 0)
    {
    // caso contrario, checa por tiles com feromonio de comida aos lados, exceto no seu tile
        lookTo(tileObj->getx(), tileObj->gety());
    } else if ((antTile->pheroList)[getAnthillIndex()] > 0)
    {
    // caso haja nenhum, checa se seu tile esta com feromonio (virar direção)
        face = mapInd((mapDir(face)+2)%4);
    } else
    {
    // caso nenhuma das condicoes anteriores, ande para frente com maior prob doq os lados
        int weight[4];
        weight[mapDir(face)] = 1;
        weight[(mapDir(face)+1)%4] = 1;
        weight[(mapDir(face)+2)%4] = 1;
        weight[(mapDir(face)+3)%4] = 1;
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