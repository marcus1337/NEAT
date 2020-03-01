#include "NEAT.h"
#include "Speciator.h"
#include <vector>

#ifndef COORDINATOR_H
#define COORDINATOR_H

class Coordinator {

public:
    std::vector<NEAT> neats;
    Speciator speciator;
    int generation;

    void init(int numIn, int numOut, int numAI);
    void evolve();
    void calcInput(int index, float* inputs);
    float* getOutput(int index);
    void setFitness(int index, int fitness);
    void save(int index, std::string filename);
    void load(std::string filename, int numAI);

};

#endif