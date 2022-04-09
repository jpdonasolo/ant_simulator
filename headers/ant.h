#pragma once

#include "entity.h"

enum antBehavior { seek, bring };
enum dir { north=0, east=1, south=2, west=3};

class Ant : public Entity {
public:
    Ant(int x, int y, int anthillIndex)
        : Entity(x, y)
        , indAnthill(anthillIndex)
        {}

    int getAnthillIndex() const { return indAnthill; }
    
    char getMarker() 
    { 
        if(mode == seek){
            return 'A'; 
        }else{
            return 'B'; 
        }
    }

    void lookTo(int x, int y)
    {
        if(x > posx)
        {
            face = east;
        }else if(x < posx)
        {
            face = west;
        }else if(y > posy)
        {
            face = south;
        }else if(y < posy)
        {
            face = north;
        }
    }

    dir face;
    antBehavior mode = seek;
private:
    const int indAnthill;
};