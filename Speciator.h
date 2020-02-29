
#include "NEAT.h"
#include "Specie.h"
#include <random>

#ifndef SPECIATOR_H
#define SPECIATOR_H

class Speciator {
    int numOut, numIn, numAI;

public:
    std::default_random_engine rng;
    static constexpr float c1 = 1.8f;
    static constexpr float c2 = 0.4f;
    static constexpr float crossChance = 0.75f;
    int totAvg = 1;
    std::vector<Specie> pool;
    std::vector<NEAT> children;
    int specieNum;

    Speciator();

    void init(int _numIn, int _numOut, int _numAI);
    int totalAvgFit();
    void speciate(std::vector<NEAT>& neats);
    void crossOver(NEAT* n1, NEAT* n2);

    void breedChild(const Specie& specie);
    bool isWeak(const Specie& o);
    void removeWeakSpecies();

    void newGeneration();
    void removeStaleSpecies();
    void cullSpecies(bool onlyOneLeft = false);
    void addToSpecies(NEAT& neat);

    bool sameSpecie(NEAT& n1, NEAT& n2);
    float weightDiff(NEAT& n1, NEAT& n2);
    float disjointDiff(NEAT& n1, NEAT& n2);

};

#endif