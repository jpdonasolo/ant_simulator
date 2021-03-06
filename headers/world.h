#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <mutex>
#include <thread>
#include <iterator>
#include <list>
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


/*
A classe World é o contêiner de tudo o que acontece na simulação.
*/
class World{
public:
    
    World(){};
    ~World();

    void print();
    void setup(int jsonIdx);
    void update();

    /*
    Transforma coordenadas cartesianas na posição da array de tiles
    */
    int posToInt(int posx, int posy);

    /*
    Entidades da simulação
    */
    std::vector<Tile*> m_chart;
    std::vector<Anthill*> m_anthills;
    std::vector<Food*> m_foods; // To be able to work with mutex array
    std::vector<Ant*> m_ants;
    std::vector<Pheromone*> m_pheromones;

    /*
    Atributos da simulação
    */
    bool running = true;
    int curIteration = 0;

    /*
    Metadados da simulação - LIDOS DO JSON
    */
    Json::Value config;

    /*
    Funções úteis às formiguinhaz
    */
    template <class EntityGrid>
    int getEntityIndex(int posx, int posy, EntityGrid entities);

    int getMaxIteration() { return config["maxIteration"].asInt(); }
    int getHeight() { return config["height"].asInt(); }
    int getWidth() { return config["width"].asInt(); }
    int getSquareSize() { return config["squareSize"].asInt(); }
    
    void addPheromone(Pheromone * pheromone);
    void draw();
private:

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
    SDL_Texture * ant2Texture;
    SDL_Texture * foodTexture;
    SDL_Texture * foodyTexture;
    SDL_Texture * notFoodTexture;
    SDL_Texture * anthillTexture;
    void setupSDL();

    /*
    Leitura dos dados necessários à execução do programa
    */
    Json::Value readJson(int jsonIdx);

    /*
    Funções para inicialização do mapa
    */
    void setupChart();
    void addAntsAndHills();
    void addFoods();

    /*
    Funções de update
    */
    template <class EntityType>
    void updateEntities(FlowController & fc, std::vector<EntityType> & entities);
    template <class EntityType>
    void updateWithThreads(std::vector<EntityType> & entities);

    /*
    Mutexes
    */
   std::mutex mutex_phero;
};

/*
 * Essas duas próximas funções fazem o update de todas instâncias de um
 * determinado tipo de entidade (feromônio, formiga ou comida) utilizando
 * um determinado número de threads.
 */

template <class EntityType>
void World::updateEntities(FlowController & fc, std::vector<EntityType> & entities)

{

    int idx;

    while (true)
    {
        try
        {
            idx = fc.next();
            entities[idx]->update();
        }
        catch (const MaxEntitiesReached & e)
        {
            break;
        }
    }
}

template <class EntityType>
void World::updateWithThreads(std::vector<EntityType> & entities)
{

    FlowController fc;
    std::vector<std::thread *> threads;

    fc.setMax(entities.size());

    for (int i = 0; i < config["nThreads"].asInt(); i++)
    {
        std::thread * thread = new std::thread
                                ([this, &fc, &entities]{this->updateEntities(fc, entities);});
        threads.push_back(thread);
    }

    for (std::thread * thread : threads)
    {
        thread->join();
        delete thread;
    }
}

/*
 * Dadas coordenadas e uma estrutura de dados contendo entidades,
 * retorna a entidade que está nessas coordenadas do mapa.
 */
template <class EntityGrid>
int World::getEntityIndex(int posx, int posy, EntityGrid entities){
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
