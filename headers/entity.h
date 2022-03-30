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
    Entity(char m) : m_marker(m) {};
    
    char getMarker() const { return m_marker; }
    virtual bool isEmpty() const {};

protected:
    const char m_marker;
};

class EmptySpace : public Entity {
public:
    EmptySpace() : Entity(' '){};

    bool isEmpty(){ return true; };
};