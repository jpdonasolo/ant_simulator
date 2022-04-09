#include <jsoncpp/json/value.h>
#include <cstdlib>
#include <iostream>

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

Food * foodFactory(const Json::Value attributes)
{
    const int x = attributes["x"].asInt();
    const int y = attributes["y"].asInt();

    const int refillTime = attributes["refillTime"].asInt();
    const int refillQuantity = attributes["refillQuantity"].asInt();

    Food * fd = new Food(x, y, refillTime, refillQuantity);

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