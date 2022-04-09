#pragma once

class Entity{
public:
    Entity(int x, int y)
    : posx(x)
    , posy(y)
    {}

    int getx() { return posx; }
    int gety() { return posy; }
    void addx() { posx++; }
    void addy() { posy++; }
    void subx() { posx--; }
    void suby() { posy--; }

    virtual char getMarker() = 0;

protected:
    int posx;
    int posy;
};