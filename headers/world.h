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
    no vetor World::m_chart, que guarda todas as entidades presentes
    no mapa
    */
    int posToInt(int posx, int posy){
        return posx + m_width * posy;
    }
 
    void addAntToChart(Ant * ant, int posx, int posy);

private:
    // Mapa
    std::vector<Entity *> m_chart;
    /*
    Vetor que rastreia todas as formigas do mundo. Será usado
    para paralelismo. Ponteiro de ponteiro pois vai apontar
    para a posição da formiga no mapa.
    */
    std::vector<Ant **> m_ants;
    const int m_width, m_height;
};


void World::print()
{
    for (int pos = 0; pos < m_chart.size(); pos++){
        
        cout << m_chart[pos]->getMarker();
        
        if ((pos + 1 ) % m_width == 0){
            cout << endl;
        };
    }
}

World::World(int width, int height)
    : m_width(width), m_height(height)
{
    m_chart.resize(m_width * m_height);
    Entity * emptySpace =  new EmptySpace();

    /*
    Todos os espaços vazios apontam para a mesma entidade 
    Entity, que representa um espaço vazio, para salvar
    memória.
    */
    for (int i = 0; i < m_chart.size(); i++){
        m_chart[i] = emptySpace;
    }
}

void World::addAntToChart(Ant * ant, int posx, int posy)
{    
    const int indexInChart = posToInt(posx, posy);
    m_chart[indexInChart] = ant;
}