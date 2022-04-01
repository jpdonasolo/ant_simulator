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


#define CONFIGURATION_PATH "config.json"

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
    int posToInt(int posx, int posy){
        return posx + m_chartWidth * posy;
    }

 

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
    Metadados da simulação - LIDOS DO JSON
    */
    int m_chartWidth, m_chartHeight;
    Json::Value config;

    /*
    Leitura dos dados necessários à execução do programa
    */
    Json::Value readJson();

    /*
    Funções para inicialização do mapa
    */
   void resizeChart();
   void addFoodSources();
};


void World::setup()
{
    config = readJson();

    resizeChart();
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
    const int height = config["height"].asInt();
    const int width = config["width"].asInt();

    m_chart.resize(height * width);
}

void World::addFoodSources()
{
    Json::Value foodSourcesInfo = config["foodSources"];

    for (Json::Value foodSourceInfo : foodSourcesInfo)
    {
        FoodSource * fd = foodSourceMaker(foodSourceInfo);
        m_foodSources.push_back(*(fd));
        
    }

    for (FoodSource foodSourceInfo : m_foodSources)
    {
        std::cout << foodSourceInfo.gety() << std::endl;
    }

}