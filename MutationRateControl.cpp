#include "MutationRateControl.h"
#include <algorithm>

using namespace NTE;

bool MutationRateControl::shouldIncreaseMutationRate() {
    return numSuccesfulGenerations > 1;
}
bool MutationRateControl::shouldDecreaseMutationRate() {
    return numSuccesfulGenerations < 1;
}


void MutationRateControl::modifyMutationRate(float& mutationChance, std::vector<NEAT>& trees) {
    for (const auto& tree : trees)
        currentBestFitness = std::max(currentBestFitness, tree.fitness);

    if (currentBestFitness > priorBestFitness)
        numSuccesfulGenerations++;

    if (numEvaluatedGenerations == 0) {
        if (shouldIncreaseMutationRate())
            mutationChance *= 2.0f;
        if (shouldDecreaseMutationRate())
            mutationChance /= 2.0f;
        mutationChance = roundf(mutationChance * 1000) / 1000;
        mutationChance = std::clamp(mutationChance, 1.f, 10.0f);
        numSuccesfulGenerations = 0;
    }

    priorBestFitness = currentBestFitness;
    numEvaluatedGenerations = (numEvaluatedGenerations + 1) % 5;
}