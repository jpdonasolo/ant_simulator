#pragma once

#include <iostream>

/*
A ideia da classe entity é ser a base para todos os objetos físicos do mapa
(formigueiros, formigas e comidas; ferormônios não). A classe base serve 
também como espaço vazio no mapa.

A keyword virtual serve para algumas questões envolvendo herança. Posso
explicar depois.

@ Marker: Símbolo a ser impresso no console representando aquela entidade
*/
class Entity{
public:
    Entity() : marker(' ') {};
    Entity(char m) : marker(m) {};
    
    char getMarker() const { return marker; }
    virtual bool isEmpty() const { return true; };

protected:
    const char marker;
};