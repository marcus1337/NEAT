#include "NEATInterface_C.h"
#include <utility>
#include <limits.h>
#include "Coordinator.h"
#include <vector>


using namespace std;

Coordinator coordinator;

void init(int inputs, int outputs, int numAI)
{
    coordinator.init(inputs, outputs, numAI);
}

void processInput(int index, float* inputs)
{
    coordinator.calcInput(index, inputs);
}

void processInputAllNEATs(float* inputs)
{
    coordinator.calcInputAll(inputs);
}

float* getOutput(int index)
{
    return coordinator.getOutput(index);
}

void setFitness(int index, int fitness) {
    coordinator.setFitness(index, fitness);
}

void evolve() {
    coordinator.evolve();
}

void save(int index, char* fileID) {
    coordinator.save(index, fileID);
}

void load(char* filename, int numAI) {
    coordinator.load(filename, numAI);
}