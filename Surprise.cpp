#include "Surprise.h"
#include <limits>

using namespace NTE;


void Surprise::initMean(std::vector<NEAT>& NEATs) {
    numElements = (int)NEATs[0].observedBehaviors.size();
    numNEATs = (int)NEATs.size();
    mean = getMean(NEATs);
    numGenerations = 1;
}

Behavior<float> Surprise::getMean(std::vector<NEAT>& NEATs) {
    Behavior<float> result;
    result.set({ 0,0,0 });
    for (const auto& NEAT : NEATs)
        result += NEAT.observedBehaviors;
    result /= numNEATs;
    return result;
}

void Surprise::updateMean(std::vector<NEAT>& NEATs) {
    if (mean.empty())
        initMean(NEATs);

    Behavior<float> latestMean = getMean(NEATs);
    mean += (latestMean - mean) / numGenerations;
    numGenerations++;
}

bool Surprise::shouldSurprise(float maxDistance, float totalPot) {
    if (maxDistance <= 0.5f || totalPot < 1.f || effect <= 0.0001f)
        return false;
    return true;
}

void Surprise::addSurpriseFitness(std::vector<NEAT>& NEATs) {
    updateMean(NEATs);

    std::vector<float> distances = getDistances(NEATs);
    float maxDistance = getMaxDistance(distances);
    float totalPot = getTotalPot(NEATs);

    if (!shouldSurprise(maxDistance, totalPot))
        return;

    removeFitness(NEATs);
    distributeSurpriseFitness(NEATs, totalPot, maxDistance, distances);
}

void Surprise::removeFitness(std::vector<NEAT>& NEATs) {
    for (auto& NEAT : NEATs)
        NEAT.fitness -= (int)((float)NEAT.fitness*effect);
}

std::vector<float> Surprise::getDistances(std::vector<NEAT>& NEATs) {
    std::vector<float> distances(NEATs.size());
    for (size_t i = 0; i < NEATs.size(); i++)
        distances[i] = mean.distance(NEATs[i].observedBehaviors);
    return distances;
}

float Surprise::getMaxDistance(std::vector<float>& distances) {
    float maxDistance = std::numeric_limits<float>::min();
    for (size_t i = 0; i < distances.size(); i++)
        maxDistance = std::fmaxf(distances[i], maxDistance);
    return maxDistance;
}

void Surprise::distributeSurpriseFitness(std::vector<NEAT>& NEATs, float totalPot, float maxDistance,
    std::vector<float>& distances) {

    std::vector<float> normalizedDistances = getNormalizedDistances(distances, maxDistance);
    float maxScore = getMaxScore(totalPot, normalizedDistances);
    for (size_t i = 0; i < NEATs.size(); i++) {
        NEATs[i].fitness += (int)(maxScore*normalizedDistances[i]);
    }
}

std::vector<float> Surprise::getNormalizedDistances(std::vector<float>& distances, float maxDistance) {
    std::vector<float> normalizedDistances = distances;
    for (size_t i = 0; i < distances.size(); i++)
        normalizedDistances[i] /= maxDistance;
    return normalizedDistances;
}

float Surprise::getMaxScore(float totalPot, std::vector<float>& normalizedDistances) {
    float sumNormalizedDistances = std::accumulate(normalizedDistances.begin(), normalizedDistances.end(), 0.f);
    return totalPot / sumNormalizedDistances;
}

float Surprise::getTotalPot(std::vector<NEAT>& NEATs) {
    float totalPot = 0;
    for (auto& NEAT : NEATs)
        totalPot += NEAT.fitness*effect;
    return totalPot;
}
