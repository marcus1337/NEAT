#include "Coordinator.h"
//#pragma GCC optimize ("O3")
//#pragma GCC target ("avx")
#include "mimalloc-override.h"
#include "mimalloc-new-delete.h"

#include <ctime>
#include "Innovator.h"
#include "IOstuff.h"
#include <algorithm>

#include <iostream>
#include <future>

#include "StandardStrategy.h"

Coordinator::Coordinator() {
    speciator = new StandardStrategy();
}

Coordinator::~Coordinator() {
    delete speciator;
}

void Coordinator::init(int numIn, int numOut, int numAI) {
    generation = 1;
    Innovator::getInstance().reset();
    Innovator::getInstance().nodeNum = numIn + numOut + 1;

    (*neatBuffer.neats).clear();
    for (int i = 0; i < numAI; i++)
        (*neatBuffer.neats).push_back(NEAT(numIn, numOut));
    neatBuffer.setBuffSize(numAI);

    speciator->init(numIn, numOut, numAI);
}

void Coordinator::evolve() {
    generation++;
    speciator->speciate(*neatBuffer.neats, *neatBuffer.oldNeats);
    neatBuffer.swapBuffers();
}

void Coordinator::calcInput(int index, float* inputs) {
    (*neatBuffer.neats)[index].calculateOutput(inputs);
}

void Coordinator::calcInputAll(float* inputs) {
    std::vector<std::future<void>> futures;
    for (int i = 0; i < neatBuffer.neats->size(); i++) {
        futures.push_back(std::async(std::launch::async | std::launch::deferred,
            std::bind(&Coordinator::calcInput, *this, i, inputs)));
    }
    for (auto &fut : futures)
        fut.wait();
}

float* Coordinator::getOutput(int index) {
    return (*neatBuffer.neats)[index].outputs.data();
}

void Coordinator::setFitness(int index, int fitness) {
    (*neatBuffer.neats)[index].fitness = fitness * speciator->numAI;
}

void Coordinator::save(int index, std::string filename) {
    SaveData saveData((*neatBuffer.neats)[index], filename, generation);
    saveData.takenNodeIDs = Innovator::getInstance().getAllNodeIDs();
    IOstuff::save(saveData);
}

void Coordinator::initNEATBuffers(NEAT& neat, int numAI) {
    (*neatBuffer.neats).clear();
    speciator->init(neat.numIn, neat.numOut, numAI);
    neatBuffer.setBuffSize(numAI);
    for (int i = 0; i < numAI; i++)
        (*neatBuffer.neats).push_back(neat);
}

void Coordinator::setMaxInnovationNumber(NEAT& neat) {
    for (auto const& x : neat.nodes) {
        int id = x.second.getID();
        Innovator::getInstance().nodeNum = std::max<int>(Innovator::getInstance().nodeNum, id);
    }
    Innovator::getInstance().nodeNum += 1;
}

void Coordinator::load(std::string filename, int numAI) {
    SaveData saveData = IOstuff::load(filename);
    generation = saveData.generation;
    NEAT& neat = saveData.neat;
    initNEATBuffers(neat, numAI);
    setMaxInnovationNumber(neat);
    Innovator::getInstance().setTakenNodeIDs(saveData.takenNodeIDs);
}

void Coordinator::changeEvolveStrategy() {

}