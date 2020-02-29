#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random {
public:

    static std::random_device   mRd;
    static std::mt19937         mRng;
    static std::uniform_int_distribution<> boolDist;

};

#endif // !RANDOM_H