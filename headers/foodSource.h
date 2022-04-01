#pragma once


class FoodSource {
public:
    FoodSource(int x, int y, int rt, int qt)
        : posx(x)
        , posy(y)
        , refillTime(rt)
        , refillQuantity(qt)
        {}

    int getx() const { return posx; };
    int gety() const { return posy; };
    int getRefillTime() const { return refillTime; };
    int getRefillQuantity() const { return refillQuantity; };

private:
    const int posx;
    const int posy;
    const int refillTime;
    const int refillQuantity;
};