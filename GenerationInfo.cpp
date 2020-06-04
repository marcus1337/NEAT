#include "GenerationInfo.h"

#include <algorithm>
#include <limits>

using namespace NTE;

GenerationInfo::GenerationInfo() {
    averageFitness = 0.f;
    maxFitness = std::numeric_limits<int>::min();
    minFitness = std::numeric_limits<int>::max();
    generation = 0;
    numNEATs = 0;
}
void GenerationInfo::saveData(std::ofstream& stream, std::vector<NEAT>& NEATs) {
    storeData(NEATs);
    stream << maxFitness << '\n';
    stream << minFitness << '\n';
    stream << averageFitness << '\n';
    stream << generation << '\n';
    stream << numNEATs << '\n';
    for (int fitness : fitnesses)
        stream << fitness << '\n';
}

void GenerationInfo::storeData(std::vector<NEAT>& NEATs) {
    numNEATs = (int)NEATs.size();
    for (size_t i = 0; i < NEATs.size(); i++) {
        averageFitness += NEATs[i].fitness;
        fitnesses.push_back(NEATs[i].fitness);
        maxFitness = std::max(maxFitness, NEATs[i].fitness);
        minFitness = std::min(minFitness, NEATs[i].fitness);
    }
    averageFitness /= NEATs.size();
}

void GenerationInfo::loadData(std::ifstream& stream) {
    stream >> maxFitness;
    stream >> minFitness;
    stream >> averageFitness;
    stream >> generation;
    stream >> numNEATs;
    for (size_t i = 0; i < (size_t)numNEATs; i++) {
        int fitness;
        stream >> fitness;
        fitnesses.push_back(fitness);
    }
}