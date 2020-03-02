#include "Coordinator.h"
#include <ctime>
#include "Innovator.h"
#include "IOstuff.h"
#include <algorithm>

void Coordinator::init(int numIn, int numOut, int numAI) {
    generation = 1;
    Innovator::getInstance().reset();
    for (int i = 0; i <= numIn + numOut; i++)
        Innovator::getInstance().getAnyNodeNum();

    for (int i = 0; i < numAI; i++) {
        (*neatBuffer.neats).push_back(NEAT(numIn, numOut));
    }
    speciator.init(numIn, numOut, numAI);
}

void Coordinator::evolve() {
    generation++;
    neatBuffer.prepareNewGeneration();
    speciator.speciate(*neatBuffer.neats, *neatBuffer.oldNeats);
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

void Coordinator::load(std::string filename, int numAI) {
    SaveData saveData = IOstuff::load(filename);
    generation = saveData.generation;
    NEAT neat = saveData.neat;
    (*neatBuffer.neats).clear();
    for (int i = 0; i < numAI; i++) {
        (*neatBuffer.neats).push_back(neat);
    }
    for (auto const& x : neat.nodes) {
        int id = x.second.getID();
        Innovator::getInstance().nodeNum = std::max<int>(Innovator::getInstance().nodeNum, id);
    }
    Innovator::getInstance().nodeNum += 1;
    Innovator::getInstance().setTakenNodeIDs(saveData.takenNodeIDs);

    speciator.init(neat.numIn, neat.numOut, numAI);
}