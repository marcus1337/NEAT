#include "NEAT.h"
#include <vector>

#ifndef SPECIE_H
#define SPECIE_H

struct Specie {
    int id;
    std::vector<NEAT*> neats;
    NEAT* getRandomNeat();
    int topFitness = 0;
    int averageFitness = 0;

    void calcAvgFit();
    Specie();
    Specie(int _id);

    bool operator < (const Specie &right) const;
    bool operator==(const Specie& rhs) const;
};

#endif