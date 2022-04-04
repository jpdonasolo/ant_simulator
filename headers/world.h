#pragma once

#include <iostream>
#include <vector>
#include <fstream>

// https://www.codeproject.com/articles/1102603/accessing-json-data-with-cplusplus
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#include "ant.h"
#include "anthill.h"
#include "foodSource.h"
#include "pheromone.h"
#include "tile.h"

#include "funcs.h"


#define CONFIGURATION_PATH "../config.json"

/*
A classe World é o contêiner de tudo o que acontece na simulação.
*/
class World{
public:
    
    World(){};
    ~World(){};

    void print();
    void setup();

    /*
    Fora da classe World, trabalhamos com duas coordenadas, x e 
    y, por facilidade. Internamente, a classe converte essas duas
    coordenadas para o índice representativo daquelas coordenadas
    no vetor World::m_chart, que guarda o mapa
    */
    int posToInt(int posx, int posy);

 

private:

    /*
    Entidades da simulação
    */
    std::vector<Tile> m_chart;
    std::vector<Ant> m_ants;
    std::vector<Anthill> m_anthills;
    std::vector<FoodSource> m_foodSources;
    std::vector<Pheromone> m_pheromones;

    /*
    Grid para exibição do mapa
    */
   std::vector<char> m_grid;

    /*
    Metadados da simulação - LIDOS DO JSON
    */
    Json::Value config;

    int getHeight() { return config["height"].asInt(); }
    int getWidth() { return config["width"].asInt(); }

    /*
    Leitura dos dados necessários à execução do programa
    */
    Json::Value readJson();

    /*
    Funções para inicialização do mapa
    */
    void resizeChart();
    void addAntsAndHills();
    void addFoodSources();

    /*
    Funções para exibição do mapa
    */
   void setupGrid();

   template <typename entityType>
   void addEntitiesToGrid(std::vector<entityType> entities);

};


void World::setup()
{
    config = readJson();

    setupGrid();

    resizeChart();
    addAntsAndHills();
    addFoodSources();
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

void World::resizeChart()
{
    const int height = getHeight();
    const int width = getWidth();

    m_chart.resize(height * width);
}

void World::setupGrid()
{
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
        Anthill * ah = anthillMaker(anthillInfo, anthillIndex);
        m_anthills.push_back(*(ah));

        for(int i = 0; i<(*ah).getPopu(); i++){
            Ant * ant = new Ant((*ah).getx(), (*ah).gety(), anthillIndex);
            m_ants.push_back(*(ant));
        }

        ++anthillIndex;
    }
}

void World::addFoodSources()
{
    Json::Value foodSourcesInfo = config["foodSources"];

    for (Json::Value foodSourceInfo : foodSourcesInfo)
    {
        FoodSource * fd = foodSourceMaker(foodSourceInfo);
        m_foodSources.push_back(*(fd));
        
    }
}

int World::posToInt(int posx, int posy){
    return posx + getWidth() * posy;
}

void World::print()
{

    addEntitiesToGrid<Ant>(m_ants);
    addEntitiesToGrid<FoodSource>(m_foodSources);
    addEntitiesToGrid<Anthill>(m_anthills);
    
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
}

template <typename entityType>
void World::addEntitiesToGrid(std::vector<entityType> entities)
{
    for (int idx = 0; idx < entities.size(); idx++)
    {
        Entity * entity = &(entities[idx]);
        m_grid[(entity->getx() + 1) + (getWidth() + 2)*(entity->gety()+1)] = entity->getMarker();
    }
}