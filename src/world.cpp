#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// https://www.codeproject.com/articles/1102603/accessing-json-data-with-cplusplus
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#include "ant.h"
#include "anthill.h"
#include "food.h"
#include "pheromone.h"
#include "tile.h"
#include "world.h"
#include "utils.h"



void World::setup()
{
    config = readJson();

    setupGrid();

    setupChart();
    addAntsAndHills();
    addFoods();
}

Json::Value World::readJson()
{
    std::ifstream configFile (CONFIGURATION_PATH);
    
    Json::Reader reader;
    Json::Value configData;

    if (!reader.parse(configFile, configData, false))
    {
        std::cout << "O arquivo não pode ser lido. Verifique o caminho especificado\n";
    }

    return configData;
    
}

void World::setupChart()
{   
    const int height = getHeight();
    const int width = getWidth();

    const int nAnts = config["anthills"].size();
    
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            Tile * tilePointer = tileFactory(j, i, nAnts);
            m_chart.push_back(tilePointer);
        }
    }
}

void World::setupGrid()
{   
    const int heightPlusWalls = getHeight() + 2;
    const int widthPlusWalls = getWidth() + 2;

    for(int cury = 0; cury < heightPlusWalls; cury++)
    {
        for(int curx = 0; curx < widthPlusWalls; curx++)
        {
            if ((curx == 0 && cury == 0) || (curx == widthPlusWalls - 1 && cury == heightPlusWalls - 1))
            {
                m_grid_base.push_back('/');
            }
            else if ((curx == widthPlusWalls - 1 && cury == 0) || (curx == 0 && cury == heightPlusWalls - 1))
            {
                m_grid_base.push_back('\\');
            }
            else if ((curx > widthPlusWalls - 2) || (curx < 1))
            {
                m_grid_base.push_back('|');
            }
            else if ((cury < 1) || (cury > heightPlusWalls - 2))
            {
                m_grid_base.push_back('-');
            }
            else
            {
                m_grid_base.push_back(' ');
            }
        }
    }
}

void World::addAntsAndHills()
{
    Json::Value anthillsInfo = config["anthills"];

    int anthillIndex = 0;
    for (Json::Value anthillInfo : anthillsInfo)
    {
        Anthill * ah = anthillFactory(anthillInfo, anthillIndex);
        m_anthills.push_back(ah);

        Tile * anthillTile = m_chart[posToInt((*ah).getx(),(*ah).gety())];
        anthillTile->isAnthill = true;

        for(int i = 0; i < ah->getPopu(); i++){
            Ant * ant = new Ant(ah->getx(), ah->gety(), anthillIndex, this);
            int uniform[4] = {1,1,1,1};
            ant->face = randDir(uniform);
            m_ants.push_back(ant);
        }

        ++anthillIndex;
    }
}

void World::addFoods()
{
    Json::Value foodsInfo = config["foods"];

    for (Json::Value foodInfo : foodsInfo)
    {
        Food * fd = foodFactory(foodInfo, this);
        m_foods.push_back(fd);

        Tile * foodTile = m_chart[posToInt(fd->getx(),fd->gety())];

        if(fd->currentFood>0)
        {
            foodTile->hasFood = true;
        }
    }
}

int World::posToInt(int posx, int posy)
{
    return posx + getWidth() * posy;
}

void World::print()
{   
    // Esse caractere é traduzido para o comando de clear screen
    // do sistema operacional
    // https://stackoverflow.com/questions/17335816/clear-screen-using-c
    // Testado em Windows e Linux
    std::cout << "\033[2J\033[1;1H";

    const int widthPlusWalls = getWidth() + 2;
    const int heightPlusWalls = getHeight() + 2;

    std::vector<std::string> m_grid(widthPlusWalls*heightPlusWalls,""); 
    for(int i = 0; i < heightPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            m_grid[(i*widthPlusWalls)+j] += m_grid_base[(i*widthPlusWalls)+j];
        };
    };

    addEntitiesToGrid(m_pheromones, m_grid);
    addEntitiesToGrid(m_ants, m_grid);
    addEntitiesToGrid(m_foods, m_grid);
    addEntitiesToGrid(m_anthills, m_grid);

    for(int i = 0; i < heightPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            std::cout << m_grid[i*(widthPlusWalls) + j];
            if(j==widthPlusWalls-1){
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

template <class ListOrVector>
void World::addEntitiesToGrid(ListOrVector entities, std::vector<std::string> & m_grid)
{   
    for (auto it : entities)
    {   
        std::string marker = "";
        marker += it->getColor();
        marker += it->getMarker();
        marker += reset;
        m_grid[(it->getx() + 1) + (getWidth() + 2)*(it->gety()+1)] = marker;
    }
}

void World::update()
{   
    updateWithThreads(m_pheromones);

    std::vector<Pheromone*> newPhero;
    for (auto phero : m_pheromones)
    {
        if (!(phero->toRemove)){ newPhero.push_back(phero); }
    }
    m_pheromones = newPhero;

    updateWithThreads(m_ants);
    updateWithThreads(m_foods);

    return;
}


void World::addPheromone(Pheromone * phero)
{
    std::lock_guard<std::mutex> lg(mutex_phero);
    m_pheromones.push_back(phero);
}