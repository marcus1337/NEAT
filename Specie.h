#pragma once
#include "NEAT.h"
#include <vector>

struct Specie {
    int id;
    std::vector<NEAT*> neats;
    int topFitness = 0;
    int averageFitness = 0;

    void calcAvgFit();
    Specie();
    Specie(int _id);

    bool operator < (const Specie &right) const;
    bool operator==(const Specie& rhs) const;
};