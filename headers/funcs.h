#pragma once

#include <jsoncpp/json/value.h>

#include "foodSource.h"
#include "ant.h"
#include "anthill.h"

FoodSource * foodSourceMaker(const Json::Value attributes)
{
    const int x = attributes["x"].asInt();
    const int y = attributes["y"].asInt();

    const int refillTime = attributes["refillTime"].asInt();
    const int refillQuantity = attributes["refillQuantity"].asInt();

    FoodSource * fd = new FoodSource(x, y, refillTime, refillQuantity);

    return fd;

}

Anthill * anthillMaker(const Json::Value attributes, int anthillIndex)
{
    const int x = attributes["x"].asInt();
    const int y = attributes["y"].asInt();
    const int nAnts = attributes["nAnts"].asInt();

    Anthill * ah = new Anthill(x, y, anthillIndex, nAnts);

    return ah;
}