#pragma once

#include <jsoncpp/json/value.h>
#include <cstdlib>
#include <iostream>
#include <mutex>

enum dir { north=0, east=1, south=2, west=3};
class Food;
class World;
class Anthill;
class Tile;

dir mapInd(int index);
int mapDir(dir direction);
char getDirChar(dir direction);
dir randDir(int weight[4]);
Food * foodFactory(const Json::Value attributes, World * worldP);
Anthill * anthillFactory(const Json::Value attributes, int anthillIndex);
Tile * tileFactory(int x, int y, int nAnts);
double dist(int x1, int y1, int x2, int y2);


const std::string red1("\033[0;31m");
const std::string green1("\033[1;32m");
const std::string yellow1("\033[1;33m");
const std::string cyan1("\033[0;36m");
const std::string magenta1("\033[0;35m");
const std::string reset1("\033[0m");
const int TOTALCOLORS = 4;
std::string color(int idx);


class MaxEntitiesReached{};


class FlowController
{
public:
    FlowController() { reset(); }
    
    void reset();
    int next();
    void setMax(int m) { max = m; };

private:
    int max;
    int current;

    std::mutex m;
};