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
};


void World::setup()
{
    config = readJson();

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
    std::vector<std::string> grid;

    const int height = getHeight();
    const int width = getWidth();

    const int heightPlusWalls = height + 2;
    const int widthPlusWalls = width + 2;


    for(int i = 0; i < widthPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            if(i==0 || i==width+1 || j==0 || j==height+1){
                grid.push_back("X");
            }else{
                grid.push_back(" ");
            }
        }
    }

    /*
    A = formigas
    H = formigueiro
    F = fonte de comida
    X = fora do limite do mapa
    */
    for (Ant antInfo : m_ants)
    { 
        grid[(antInfo.getx() + 1) + (widthPlusWalls)*(antInfo.gety()+1)] = "A";
    } 

    for (Anthill anthillInfo : m_anthills)
    { 
        grid[(anthillInfo.getx() + 1) + (widthPlusWalls)*(anthillInfo.gety()+1)] = "H";
    } 

    for (FoodSource foodSourceInfo : m_foodSources)
    { 
        grid[(foodSourceInfo.getx() + 1) + (widthPlusWalls)*(foodSourceInfo.gety()+1)] = "F";
    }

    for(int i = 0; i < widthPlusWalls; ++i){
        for(int j = 0; j < widthPlusWalls; ++j){
            std::cout << grid[i*(heightPlusWalls) + j];
            if(j==height+1){
                std::cout << std::endl;
            }
        }
    }
}