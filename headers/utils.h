#pragma once

#include <jsoncpp/json/value.h>
#include <cstdlib>
#include <iostream>

#include "food.h"
#include "ant.h"
#include "anthill.h"
#include "tile.h"


dir mapInd(int index);
int mapDir(dir direction);
char getDirChar(dir direction);
dir randDir(int weight[4]);
Food * foodFactory(const Json::Value attributes, World * worldP);
Anthill * anthillFactory(const Json::Value attributes, int anthillIndex);
Tile * tileFactory(int x, int y, int nAnts);
double dist(int x1, int y1, int x2, int y2);