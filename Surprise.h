#include <vector>
#include "NEAT.h"
#include "Behavior.h"

#ifndef SURPRISE_H
#define SURPRISE_H

namespace NTE {
    class Surprise {
        int numElements = -1;
        int numNEATs = -1;
        int numGenerations;
        void initMean(std::vector<NEAT>& NEATs);
        void updateMean(std::vector<NEAT>& NEATs);
        Behavior<float> getMean(std::vector<NEAT>& NEATs);
        Behavior<float> mean;
        float getTotalPot(std::vector<NEAT>& NEATs);
        void distributeSurpriseFitness(std::vector<NEAT>& NEATs, float totalPot, float maxDistance,
            std::vector<float>& distances);
        std::vector<float> getDistances(std::vector<NEAT>& NEATs);
        std::vector<float> getNormalizedDistances(std::vector<float>& distances, float maxDistance);
        float getMaxDistance(std::vector<float>& distances);
        float getMaxScore(float totalPot, std::vector<float>& normalizedDistances);
        void removeFitness(std::vector<NEAT>& NEATs);

        bool shouldSurprise(float maxDistance, float totalPot);
    public:

        float effect = 0.2f;
        void addSurpriseFitness(std::vector<NEAT>& NEATs);
        void reset();
    };
}


#endif