#pragma once

#include "entity.h"
#include "semaphore.h"

#include <mutex>
#include <algorithm> // max function

class Food : public Entity {
public:
    Food(int x, int y, int rt, int qt, int iq, const int numSticks)
        // numSeats = numSticks = number of ants in food source
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
            std::mutex s[numSticks];
            int state[numSticks];
        }

    int getRefillTime() const { return refillTime; };
    int getRefillQuantity() const { return refillQuantity; };
    void decreaseFoodCount()
    {
        std::lock_guard<std::mutex> lck(foodCounterMutex);
        currentFood = std::max(0, currentFood-1);
    }

    char getMarker() { return 'F'; }
    
    int currentTime;
    std::mutex foodCounterMutex;
    int currentFood;
    const int numSeats;

    // This semaphore is used to keep track of how many ants have entered
    // the food source
    semaphore seatsAvailable;

    // each ant gets a seat at the table
    std::mutex * seats; // array

    // ants use two sticks to eat, but they must aquire each
    // one of them individually
    std::mutex * sticks; // array

    // variables for philosophers dinner solution
    int * state;
    std::mutex * s;
    std::mutex m;

private:
    const int refillTime;
    const int refillQuantity;
};
