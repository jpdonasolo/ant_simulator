#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <mutex>
#include <thread>
#include <iterator>
#include <list>

// https://www.codeproject.com/articles/1102603/accessing-json-data-with-cplusplus
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>

#include "ant.h"
#include "anthill.h"
#include "food.h"
#include "pheromone.h"
#include "tile.h"

#include "utils.h"


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
    void update();

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
    std::vector<Anthill> m_anthills;
    std::vector<Food> m_foods;
    std::list<Ant> m_ants;
    std::list<Pheromone> m_pheromones;

    /*
    Threads
    */
    std::vector<std::thread *> m_threads;
    void setupThreads();

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
    void setupChart();
    void addAntsAndHills();
    void addFoods();


    /*
    Funções para exibição do mapa
    */
    void setupGrid();
    
    template <class ListOrVector>
    void addEntitiesToGrid(ListOrVector entities);

    
    /*
    Funções úteis às formiguinhaz
    */
    void leavePhero(Ant * ant); 
    template <class EntityGrid>
    int getEntityIndex(int posx, int posy, EntityGrid entities);
    void checkFood(Ant & ant);
    void checkAnthill(Ant & ant);
    bool checkInvalidCoordinates(int posx, int posy);
    void look(Ant & ant);
    void walk(Ant & ant);

    /*
    Funções de update para o próximo tick
    */
    void updateAnt();
};
