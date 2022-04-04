#include <jsoncpp/json/value.h>

#include "food.h"
#include "ant.h"
#include "anthill.h"
#include "utils.h"
#include "tile.h"

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

Tile * tileFactory(int nAnts)
{
    Tile * t = new Tile(nAnts);
    return t;
}