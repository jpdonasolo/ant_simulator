#pragma once

class Anthill {
public:
    Anthill(int x, int y, int anthillIndex, int nAnts)
        : posx(x)
        , posy(y)
        , ind(anthillIndex)
        , popu(nAnts)
        {}

    int getx() const { return posx; };
    int gety() const { return posy; };
    int getindex() const { return ind; };
    int getpopu() const { return popu; };

private:
    const int posx;
    const int posy;
    const int ind;
    int popu;
};