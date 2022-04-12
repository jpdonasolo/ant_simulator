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
    
    setupThreads();
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

void World::setupThreads()
{
    const int nThreads = config["nThreads"].asInt();
    int curThread = nThreads;

    while (curThread--)
    {       
        // TEMPORARIO PARA TESTAR THREADS
        // https://stackoverflow.com/questions/10673585/start-thread-with-member-function
        m_threads.push_back(new std::thread(&World::update, this));
    }    

    for(std::thread * t : m_threads)
    { 
        t->join();
        delete t;
    }
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
            m_chart.push_back(*(tilePointer));
        }
    }
}

void World::setupGrid()
{   
    m_grid.clear();
    const int heightPlusWalls = getHeight() + 2;
    const int widthPlusWalls = getWidth() + 2;

    for (int curx = 0; curx < widthPlusWalls; curx++)
    {
        for (int cury = 0; cury < heightPlusWalls; cury++)
        {
            if (curx == 0 || curx == widthPlusWalls - 1
                || cury == 0 || cury == heightPlusWalls - 1)
            {
                m_grid.push_back('X');
            }
            else
            {
                m_grid.push_back(' ');
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
        m_anthills.push_back(*(ah));

        Tile * anthillTile = &(m_chart[posToInt((*ah).getx(),(*ah).gety())]);
        anthillTile->isAnthill = true;

        for(int i = 0; i < ah->getPopu(); i++){
            Ant * ant = new Ant(ah->getx(), ah->gety(), anthillIndex, this);
            int uniform[4] = {1,1,1,1};
            ant->face = randDir(uniform);
            m_ants.push_back(*(ant));
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
        m_foods.push_back(*(fd));

        Tile * foodTile = &(m_chart[posToInt((*fd).getx(),(*fd).gety())]);
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
    setupGrid();
    
    addEntitiesToGrid(m_pheromones);
    addEntitiesToGrid(m_ants);
    addEntitiesToGrid(m_foods);
    addEntitiesToGrid(m_anthills);
    
    const int widthPlusWalls = getWidth() + 2;
    const int heightPlusWalls = getHeight() + 2;

    for(int i = 0; i < widthPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            std::cout << m_grid[i*(heightPlusWalls) + j];
            if(j==heightPlusWalls-1){
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

template <class ListOrVector>
void World::addEntitiesToGrid(ListOrVector entities)
{   
    for (auto it = entities.begin() ; it != entities.end(); ++it)
    {   
        m_grid[(it->getx() + 1) + (getWidth() + 2)*(it->gety()+1)] = it->getMarker();
    }
}

void World::update()
{   
    /* Ordem de Updates
     *
     * Pheromones
     * Ants
     * Food
     * AntHill (?)
     *
     * Cada uma dessas classes deve ter um método `update`, que irá
     * utilizar outros métodos para fazer a entidade cumprir seu papel
     * naquele turno.
     *
     * Esses outros métodos devem implementar mecanismos de controle
     * para evitar uma condição de corrida.
     *
     * Como fazer o mecanismo de controle dos Tiles?
     *
     * Deve acontecer uma sincronização após o update de cada tipo de
     * objeto.
     */
    while(true)
    {
        // update pheromones
        auto pheroIt = m_pheromones.begin();
        while (pheroIt != m_pheromones.end()) 
        {
            // pheroIt is increased inside update function
            pheroIt->update(pheroIt);
        }

        // update ants
        for (Ant & ant : m_ants)
        {   
            ant.update();
        }

        // update food
        for (Food & food : m_foods)
        {
            food.update();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        print();
    }      
    return;
}