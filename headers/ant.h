#pragma once

class Ant {
public:
    Ant(int x, int y, int anthillIndex)
        : posx(x)
        , posy(y)
        , indAnthill(anthillIndex)
        {}

    int getx() const { return posx; };
    int gety() const { return posy; };
    int getindex() const { return indAnthill; };

private:
    const int posx;
    const int posy;
    const int indAnthill;
};