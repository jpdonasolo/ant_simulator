#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <mutex>
#include <thread>
#include <iterator>
#include <list>

// Primaki
#include <SDL2/SDL.h>

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
    ~World();

    void print();
    void setup();
    void update();

    /*
    Fora da classe World, trabalhamos com duas coordenadas, x e 
    y, por facilidade. Internamente, a classe converte essas duas
    coordenadas para o índice representativo daquelas coordenadas
    no vetor World::m_chart, que guarda o mapa.
    O vetor World::m_grid precisa de uma conversão diferente.
    */
    int posToInt(int posx, int posy);

    /*
    Entidades da simulação
    */
    std::vector<Tile*> m_chart;
    std::vector<Anthill*> m_anthills;
    std::vector<Food*> m_foods; // To be able to work with mutex array
    std::list<Ant*> m_ants;
    std::list<Pheromone*> m_pheromones;

    /*
    Metadados da simulação - LIDOS DO JSON
    */
    Json::Value config;

    /*
    Funções úteis às formiguinhaz
    */
    template <class EntityGrid>
    int getEntityIndex(int posx, int posy, EntityGrid entities){
    int index = -1;
    for (auto it = entities.begin() ; it != entities.end(); ++it)
    {   
        index ++;
        if((*it)->getx() == posx && (*it)->gety() == posy)
        {
            break;
        }
    }
    return index;
}

    int getHeight() { return config["height"].asInt(); }
    int getWidth() { return config["width"].asInt(); }
    int getSquareSize() { return config["squareSize"].asInt(); }
private:

    /*
    Threads
    */
    std::vector<std::thread *> m_threads;
    void setupThreads();

    /*
    Grid para exibição do mapa
    */
    std::vector<char> m_grid_base;

    /* Primaki
    SDL da exibição do mapa
    */
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * antTexture;
    SDL_Texture * foodTexture;
    SDL_Texture * anthillTexture;
    void setupSDL();
    void draw();

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
    void addEntitiesToGrid(ListOrVector entities, std::vector<std::string> & m_grid);
};
