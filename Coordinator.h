#pragma once
#include "NEAT.h"
#include "Speciator.h"
#include <vector>

class Coordinator {

public:
    std::vector<NEAT> neats;
    int _numAI, _numOut, _numIn;

    Speciator speciator;
    int generation;

    void init(int numIn, int numOut, int numAI);
    void evolve();
    void calcInput(int index, float* inputs);
    float* getOutput(int index);
    void setFitness(int index, int fitness);
    void save(int index, int fileID);
    void load(std::string filename);

};