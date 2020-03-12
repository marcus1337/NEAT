#include "NEAT.h"
#include "Speciator.h"
#include <vector>
#include "NEATDoubleBuffer.h"

#ifndef COORDINATOR_H
#define COORDINATOR_H

class Coordinator {

    void changeEvolveStrategy();

public:
    Coordinator();
    ~Coordinator();

    NEATDoubleBuffer neatBuffer;
    Speciator* speciator = nullptr;
    //Speciator speciator;
    int generation;

    void init(int numIn, int numOut, int numAI);
    void evolve();
    void calcInput(int index, float* inputs);
    void calcInputAll(float* inputs);
    float* getOutput(int index);
    void setFitness(int index, int fitness);
    void save(int index, std::string filename);
    void load(std::string filename, int numAI);

};

#endif