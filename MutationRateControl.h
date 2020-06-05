#include "NEAT.h"
#include <vector>

#ifndef MUTATIONRATECONTROL_H
#define MUTATIONRATECONTROL_H

namespace NTE {
    class MutationRateControl {

        int numEvaluatedGenerations = 1;
        int numSuccesfulGenerations = 0;
        int priorBestFitness = 0;
        int currentBestFitness = 0;
        bool shouldIncreaseMutationRate();
        bool shouldDecreaseMutationRate();

    public:

        void modifyMutationRate(float& mutationChance, std::vector<NEAT>& neats);
    };
}

#endif // !MUTATIONRATECONTRO_H