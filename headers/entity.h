#pragma once

class Entity{
public:
    Entity(int x, int y)
    : posx(x)
    , posy(y)
    {}

    int getx() { return posx; }
    int gety() { return posy; }
private:
    const int posx;
    const int posy;
};