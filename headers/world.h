#pragma once

#include <iostream>
#include <vector>

#include "ant.h"
#include "entity.h"

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
    // Mapa
    std::vector<Entity *> chart;
    /*
    Vetor que rastreia todas as formigas do mundo. Será usado
    para paralelismo. Ponteiro de ponteiro pois vai apontar
    para a posição da formiga no mapa.
    */
    std::vector<Ant **> ants;
    const int width, height;
};


void World::print()
{
    for (int pos = 0; pos < chart.size(); pos++){
        
        cout << chart[pos]->getMarker();
        
        if ((pos + 1 ) % width == 0){
            cout << endl;
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