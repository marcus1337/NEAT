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

void Coordinator::calcInput(int index, std::vector<float> inputs) {
    (*neatBuffer.neats)[index].calculateOutput(inputs);
}

std::vector<float> Coordinator::getOutput(int index) {
    return (*neatBuffer.neats)[index].outputs;
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
    getNEATsRef().clear();
    evolver.speciator.init(neat.numIn, neat.numOut, numAI);
    neatBuffer.setBuffSize(numAI);
    for (int i = 0; i < numAI; i++)
        getNEATsRef().push_back(neat);
}

void Coordinator::load(std::string filename, int numAI) {
    SaveData saveData = IOstuff::load(filename);
    generation = saveData.generation;
    NEAT& neat = saveData.neat;
    initNEATBuffers(neat, numAI);
    Innovator::getInstance().setTakenNodeIDs(saveData.takenNodeIDs);
}

std::vector<NEAT>& Coordinator::getNEATsRef() {
    return (*neatBuffer.neats);
}


void Coordinator::setBehavior(int index, std::vector<int> behaviors) {
    getNEATsRef()[index].observedBehaviors = behaviors;
}
void Coordinator::setTargetSpecies(int numTargetSpecies) {
    evolver.speciator.targetNumSpecies = numTargetSpecies;
}
void Coordinator::setSurpriseEffect(float effect) {
    evolver.surprise.effect = effect;
}
void Coordinator::saveGeneration(std::string filename) {
    ioStuff.saveGeneration(getNEATsRef(), generation, filename);
}
void Coordinator::loadGeneration(std::string filename, int _generation) {
    getNEATsRef() = ioStuff.loadGeneration(generation, filename);
}


void Coordinator::randomizePopulation(int minNodes, int maxNodes) {
    init((*neatBuffer.neats)[0].numIn, (*neatBuffer.neats)[0].numOut, evolver.speciator.numAI);
}

void Coordinator::randomizePopulationFromElites() {
    evolver.mapElites.randomElitism((*neatBuffer.neats));
}

void Coordinator::mapElites() {
    evolver.mapElites.mapOrStoreElites((*neatBuffer.neats));
}

void Coordinator::saveElites(std::string foldername) {
    ioStuff.saveElites(evolver.mapElites.eliteNEATs, foldername);
}

void Coordinator::loadElites(std::string foldername) {
    evolver.mapElites.eliteNEATs = ioStuff.loadElites(foldername);
}

void Coordinator::storeElites() {
    evolver.mapElites.storeElites((*neatBuffer.neats));
}

void Coordinator::setParentFoldername(std::string folderName) {
    ioStuff.allSavesParentFileName = folderName;
}

void Coordinator::calcEliteInput(int index, std::vector<float> inputs) {
    auto elites = evolver.mapElites.getElitesVector();
    (*elites[index]).calculateOutput(inputs);
}

std::vector<float> Coordinator::getEliteOutput(int index) {
    auto elites = evolver.mapElites.getElitesVector();
    return (*elites[index]).outputs;
}