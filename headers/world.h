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


#define CONFIGURATION_PATH "config.json"


using std::cout;
using std::endl;

/*
A classe World é o contêiner de tudo o que acontece na simulação.
*/
class World{
public:
    
    World(int width, int height);
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
    const int m_chartWidth, m_chartHeight;

    /*
    Leitura dos dados necessários à execução do programa
    */
    Json::Value readJson();
};


void World::setup()
{
    Json::Value config = readJson();
    std::string teste = config["teste"].asString();
    cout << teste << endl;
    
}

Json::Value World::readJson()
{
    std::ifstream configFile (CONFIGURATION_PATH);
    
    Json::Reader reader;
    Json::Value configData;

    if (!reader.parse(configFile, configData, false))
    {
        cout << "O arquivo não pode ser lido. Verifique o caminho especificado\n";
    }

    return configData;
    
}




World::World(int width, int height)
    : m_chartWidth(width), m_chartHeight(height)
{
    m_chart.resize(m_chartWidth * m_chartHeight);
}