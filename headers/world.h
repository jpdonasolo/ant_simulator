#pragma once

#include <iostream>
#include <vector>

#include "ant.h"
#include "entity.h"

/*
A classe World é o contêiner de tudo o que acontece na simulação.
*/

class World{
public:
    
    World(int width, int height);
    ~World(){};

    void print();

    /*
    Fora da classe World, trabalhamos com duas coordenadas, x e 
    y, por facilidade. Internamente, a classe converte essas duas
    coordenadas para o índice representativo daquelas coordenadas
    no vetor World::chart, que guarda todas as entidades presentes
    no mapa
    */
    int posToInt(int posx, int posy){
        return posx + width * posy;
    }
 
    void addAntToChart(Ant * ant, int posx, int posy);

private:
    std::vector<Entity *> chart;
    const int width, height;
};


void World::print()
{
    for (int pos = 0; pos < chart.size(); pos++){
        
        std::cout << chart[pos]->getMarker();
        
        if ((pos + 1 ) % width == 0){
            std::cout << std::endl;
        };
    }
}

World::World(int width, int height)
    : width(width), height(height)
{
    chart.resize(width * height);
    Entity * emptySpace =  new Entity();

    /*
    Todos os espaços vazios apontam para a mesma entidade 
    Entity, que representa um espaço vazio, para salvar
    memória.
    */
    for (int i = 0; i < chart.size(); i++){
        chart[i] = emptySpace;
    }
}

void World::addAntToChart(Ant * ant, int posx, int posy)
{    
    const int indexInChart = posToInt(posx, posy);
    chart[indexInChart] = ant;
}