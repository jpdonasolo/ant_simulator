#pragma once

#include "entity.h"
#include "semaphore.h"

#include <mutex>
#include <vector>


class Food : public Entity {
public:
    Food(int x, int y, int rt, int qt, int iq, const int numSticks)
        : Entity(x, y)
        , refillTime(rt)
        , refillQuantity(qt)
        , currentFood(iq)
        , currentTime(rt)
        , numSeats(numSticks)
        , seatsAvailable(numSticks)
        {
            std::mutex sticks[numSticks];
            std::mutex seats[numSticks];
        }

    int getRefillTime() const { return refillTime; };
    int getRefillQuantity() const { return refillQuantity; };

    char getMarker() { return 'F'; }
    
    int currentTime;
    int currentFood;
    const int numSeats;

    // This semaphore is used to keep track of how many ants have entered
    // the food source
    semaphore seatsAvailable;

    // each ant gets a seat at the table
    std::mutex * seats; // array

    // ants use two sticks to eat, but they must aquire each
    // one of them individually
    std::mutex * sticks;

private:
    const int refillTime;
    const int refillQuantity;
};
