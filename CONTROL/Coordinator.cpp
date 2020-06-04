#include "Coordinator.h"
//#pragma GCC optimize ("O3")
//#pragma GCC target ("avx")
//#include "mimalloc-override.h"
//#include "mimalloc-new-delete.h"

#include <ctime>
#include "../Innovator.h"
#include "IOstuff.h"
#include <algorithm>

#include <iostream>
#include <future>

using namespace NTE;

void Coordinator::init(int numIn, int numOut, int numAI) {
    generation = 1;
    Innovator::getInstance().reset();
    Innovator::getInstance().nodeNum = numIn + numOut + 1;

    (*neatBuffer.neats).clear();
    for (int i = 0; i < numAI; i++)
        (*neatBuffer.neats).push_back(NEAT(numIn, numOut));
    neatBuffer.setBuffSize(numAI);

    evolver.speciator.init(numIn, numOut, numAI);
}

void Coordinator::evolve() {
    generation++;
    evolver.makeNewGeneration(*neatBuffer.neats, *neatBuffer.oldNeats);
    neatBuffer.swapBuffers();
}

void Coordinator::calcInput(int index, float* inputs) {
    (*neatBuffer.neats)[index].calculateOutput(inputs);
}

float* Coordinator::getOutput(int index) {
    return (*neatBuffer.neats)[index].outputs.data();
}

void Coordinator::setFitness(int index, int fitness) {
    (*neatBuffer.neats)[index].fitness = fitness;
}

void Coordinator::save(int index, std::string filename) {
    SaveData saveData((*neatBuffer.neats)[index], filename, generation);
    saveData.takenNodeIDs = Innovator::getInstance().getAllNodeIDs();
    IOstuff::save(saveData);
}

void Coordinator::initNEATBuffers(NEAT& neat, int numAI) {
    (*neatBuffer.neats).clear();
    evolver.speciator.init(neat.numIn, neat.numOut, numAI);
    neatBuffer.setBuffSize(numAI);
    for (int i = 0; i < numAI; i++)
        (*neatBuffer.neats).push_back(neat);
}

void Coordinator::load(std::string filename, int numAI) {
    SaveData saveData = IOstuff::load(filename);
    generation = saveData.generation;
    NEAT& neat = saveData.neat;
    initNEATBuffers(neat, numAI);
    Innovator::getInstance().setTakenNodeIDs(saveData.takenNodeIDs);
}