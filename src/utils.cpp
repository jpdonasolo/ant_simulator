#include <jsoncpp/json/value.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <mutex>

#include "food.h"
#include "ant.h"
#include "anthill.h"
#include "utils.h"
#include "tile.h"


dir mapInd(int index)
{
    dir direction;
    switch (index)     
    {
        case 0:
            direction = north;
            break;
        case 1:
            direction = east;
            break;
        case 2:
            direction = south;
            break;
        case 3:
            direction = west;
            break;
        default:
            break;
    }
    return direction;
}

int mapDir(dir direction)
{   
    int index;
    switch (direction)
    {
        case north:
            index = 0;
            break;
        case east:
            index = 1;  
            break;
        case south:
            index = 2;
            break;
        case west:
            index = 3;
            break;
        default:
            break;
    }
    return index;
}

char getDirChar(dir direction)
{
    char c = 'A';
    switch (direction)
    {
        case north:
            c = 'N';
            break;
        case east:
            c = 'E';
            break;
        case south:
            c = 'S';
            break;
        case west:
            c = 'W';
            break;
        default:
            break;
    }
    return c;
}

dir randDir(int weight[4])
{   
    int weightSum = 0;
    for(int i = 0; i < 4; i++)
    {
        weightSum = weightSum + weight[i];
    }

    dir direction;
    double random = (rand()+0.0)/RAND_MAX;
    double sum = 0;
    for(int i = 0; i < 4; i++)
    {   
        sum = sum + (weight[i]+0.0)/weightSum;
        if(random < sum)
        {
            direction = mapInd(i);
            break;
        }
    }
    return direction;
}

Food * foodFactory(const Json::Value attributes, World * worldP)
{
    const int x = attributes["x"].asInt();
    const int y = attributes["y"].asInt();

    const int refillTime = attributes["refillTime"].asInt();
    const int refillQuantity = attributes["refillQuantity"].asInt();
    int initialQuantity = attributes["initialQuantity"].asInt();
    int numSticks = attributes["numSticks"].asInt();

    Food * fd = new Food(x, y, refillTime, refillQuantity,
                         initialQuantity, numSticks, worldP);

    return fd;

}

Anthill * anthillFactory(const Json::Value attributes, int anthillIndex)
{
    const int x = attributes["x"].asInt();
    const int y = attributes["y"].asInt();
    const int nAnts = attributes["nAnts"].asInt();

    Anthill * ah = new Anthill(x, y, anthillIndex, nAnts);

    return ah;
}

Tile * tileFactory(int x, int y, int nAnts)
{
    Tile * t = new Tile(x, y, nAnts);
    return t;
}

double dist(int x1, int y1, int x2, int y2)
{
    double d = sqrt(pow((x1-x2),2)+pow((y1-y2),2));
    return d;
}

std::string color(int idx)
{   
    if(idx==0){ return red1; };
    if(idx==1){ return green1; };
    if(idx==2){ return yellow1; };
    if(idx==3){ return cyan1; };
    if(idx==4){ return magenta1; };
    return "";
}


void FlowController::reset()
{
    max = 0;
    current = 0;
}

int FlowController::next()
{
    std::lock_guard<std::mutex> lg(m);

    if (current == max)
    {
        throw MaxEntitiesReached();
    }

    return current++;
}