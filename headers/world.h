#pragma once

#include <iostream>
#include <vector>

#include "ant.h"
#include "anthill.h"
#include "foodSource.h"
#include "pherormone.h"
#include "tile.h"


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
    std::vector<Pherormone> m_pherormones;

    /*
    Metadados da simulação - LIDOS DO JSON
    */

    const int m_chartWidth, m_chartHeight;
};


World::World(int width, int height)
    : m_chartWidth(width), m_chartHeight(height)
{
    m_chart.resize(m_chartWidth * m_chartHeight);
}