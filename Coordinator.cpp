#include "Coordinator.h"
#include <ctime>
#include "Innovator.h"
#include "IOstuff.h"
#include <algorithm>

void Coordinator::init(int numIn, int numOut, int numAI) {
    generation = 1;
    _numAI = numAI;
    _numOut = numOut;
    _numIn = numIn;
    Innovator::getInstance().reset();
    for (int i = 0; i <= numIn + numOut; i++)
        Innovator::getInstance().getAnyNodeNum();

    for (int i = 0; i < numAI; i++) {
        neats.push_back(NEAT(numIn, numOut));
    }
    speciator.init(numIn, numOut, numAI);
}

void Coordinator::evolve() {
    generation++;
    speciator.speciate(neats);
}

void Coordinator::calcInput(int index, float* inputs) {
    neats[index].topSortCalc(inputs);
}

float* Coordinator::getOutput(int index) {
    return neats[index].outputs.data();
}

void Coordinator::setFitness(int index, int fitness) {
    neats[index].fitness = fitness;
}

void Coordinator::save(int index, int fileID) {
    IOstuff::save(neats[index], fileID);
}

void Coordinator::load(std::string filename) {
    NEAT neat = IOstuff::load(filename);
    neats.clear();
    for (int i = 0; i < _numAI; i++) {
        neats.push_back(neat);
    }
    for (auto const& x : neat.nodes) {
        int id = x.second.getID();
        Innovator::getInstance().innovNodeNum = std::max<int>(Innovator::getInstance().innovNodeNum, id);
    }
    Innovator::getInstance().innovNodeNum += 1;

}