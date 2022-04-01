#pragma once

#include <jsoncpp/json/value.h>

#include "foodSource.h"

FoodSource * foodSourceMaker(const Json::Value attributes)
{
    const int x = attributes["x"].asInt();
    const int y = attributes["y"].asInt();

    const int refillTime = attributes["refillTime"].asInt();
    const int refillQuantity = attributes["refillQuantity"].asInt();

    FoodSource * fd = new FoodSource(x, y, refillTime, refillQuantity);

    return fd;

}

