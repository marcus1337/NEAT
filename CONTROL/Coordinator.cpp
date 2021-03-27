#include "Coordinator.h"


#include <ctime>
#include "../Innovator.h"
#include "IOstuff.h"
#include <algorithm>

#include <iostream>
#include <future>
#include <limits>

using namespace NTE;

IOstuff Coordinator::ioStuff;
Evolver Coordinator::evolver;

void Coordinator::init(int numIn, int numOut, int numAI) {
    evolver.mapElites.eliteNEATs.clear();
    generation = 1;
    evolver.surprise.reset();
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

void Coordinator::setBehavior(int index, std::vector<int> behaviors) {
    (*neatBuffer.neats)[index].observedBehaviors = behaviors;
}
void Coordinator::setTargetSpecies(int numTargetSpecies) {
    evolver.speciator.targetNumSpecies = numTargetSpecies;
}
void Coordinator::setSurpriseEffect(float effect) {
    evolver.surprise.effect = effect;
}
void Coordinator::saveGeneration(std::string filename) {
    ioStuff.saveGeneration((*neatBuffer.neats), generation, filename);
}
void Coordinator::loadGeneration(std::string filename, int _generation) {
    (*neatBuffer.neats) = ioStuff.loadGeneration(generation, filename);
    generation = _generation;
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

void Coordinator::loadBestElite(std::string fileName, std::string foldername) {
    NEAT neat = ioStuff.loadNEAT(foldername, fileName);
    initNEATBuffers(neat, 1);
    
    int maxVal = 0;//THIS IS A QUICK-FIX FOR THESIS----
    for (const auto& nod : neat.nodes) {
        maxVal = std::max<int>(maxVal, nod.first);
    }
    Innovator::getInstance().nodeNum = maxVal+2; //THIS IS A QUICK-FIX FOR THESIS----
}

void Coordinator::saveBestElite(std::string fileName, std::string foldername) {
    if (evolver.mapElites.eliteNEATs.empty())
        return;
    int maxFitness = std::numeric_limits<int>::min();
    NEAT* bestElite = nullptr;

    for (auto& elite : evolver.mapElites.eliteNEATs) {
        if (elite.second.fitness > maxFitness) {
            maxFitness = elite.second.fitness;
            bestElite = &elite.second;
        }
    }

    ioStuff.saveNEAT(*bestElite, foldername, fileName);
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

void Coordinator::setMaxHiddenNodes(int _maxNodes) {
    evolver.mutater.maxHiddenNodes = _maxNodes;
}

void Coordinator::insertEliteIntoGeneration(int eliteIndex, int aiIndex) {
    auto elites = evolver.mapElites.getElitesVector();
    (*neatBuffer.neats)[aiIndex] = *elites[eliteIndex];
}

/*void Coordinator::resetRecurrentState(int index) {
    (*neatBuffer.neats)[index].resetRecurrentState();
}*/

int Coordinator::getNumElites() {
    return evolver.mapElites.getNumElites();
}
int Coordinator::getNumElitesOfUniqueDimensionValue(int dimension) {
    return evolver.mapElites.getNumElitesOfUniqueDimensionValue(dimension);
}
int Coordinator::getNumElitesOfDimensionWithValue(int dimension, int value) {
    return evolver.mapElites.getNumElitesOfDimensionWithValue(dimension, value);
}
int Coordinator::getNumElitesOfDimensionWithinThreshold(int dimension, int low, int high) {
    return evolver.mapElites.getNumElitesOfDimensionWithinThreshold(dimension, low, high);
}

std::vector<int> Coordinator::getBestEliteBehavior() {
    std::vector<int> result;
    if (evolver.mapElites.eliteNEATs.empty())
        return result;
    int maxFitness = std::numeric_limits<int>::min();
    NEAT* bestElite = nullptr;
    result = std::vector<int>(3);
    for (auto& elite : evolver.mapElites.eliteNEATs) {
        if (elite.second.fitness > maxFitness) {
            maxFitness = elite.second.fitness;
            bestElite = &elite.second;
            result[0] = std::get<0>(elite.first);
            result[1] = std::get<1>(elite.first);
            result[2] = std::get<2>(elite.first);
        }
    }
    return result;
}

void Coordinator::setMutationRates(float newNodeRate, float newLinkRate, float randomizeLinkRate, float mutateLinkRate, float enableDisableLinkRate, bool enableExtraMutationRate) {
    evolver.mutater.enableDisableLinkRate = enableDisableLinkRate;
    evolver.mutater.newNodeRate = newNodeRate;
    evolver.mutater.newLinkRate = newLinkRate;
    evolver.mutater.randomizeLinkRate = randomizeLinkRate;
    evolver.mutater.mutateLinkRate = mutateLinkRate;
    evolver.mutater.isExtraMutationActive = enableExtraMutationRate;
}