#include "Specie.h"
#include "Utils.h"
using namespace NTE;
void Specie::calcAvgFit() {

    int total = 0;
    for (const auto& n : neats) {
        total += n->fitness;
    }
    averageFitness = total / neats.size();
}

Specie::Specie() : id(-1) {}
Specie::Specie(int _id) : id(_id) {}

NEAT* Specie::getRandomNeat() {
    if (neats.empty())
        return nullptr;
    int index = Utils::randi(0, neats.size() - 1);
    return neats[index];
}

float Specie::getSpecieStrength(int numSpecies, int totalAverageFitness) const {
    if ((averageFitness > 0 && totalAverageFitness > 0) ||
        (averageFitness < 0 && totalAverageFitness < 0))
        return ((float)averageFitness / totalAverageFitness)*numSpecies;
    if (averageFitness >= totalAverageFitness)
        return 1.f;
    return 0.f;
}

bool Specie::operator < (const Specie &right) const
{
    return id < right.id;
}
bool Specie::operator==(const Specie& rhs) const { return this->id == rhs.id; }

void Specie::setDiscreteProbabilityDistribution() {
    std::vector<float> probabilityDistribution(neats.size());
    int totalFitness = 0;
    for (size_t i = 0; i < neats.size(); i++)
        totalFitness += neats[i]->fitness;
    if (totalFitness <= 0)
    {
        discreteProbabilityDistribution = { 1 };
        return;
    }
    for (size_t i = 0; i < neats.size(); i++)
        probabilityDistribution[i] = ((float)neats[i]->fitness / totalFitness)*1000.f;
    discreteProbabilityDistribution.clear();
    std::transform(probabilityDistribution.begin(), probabilityDistribution.end(), std::back_inserter(discreteProbabilityDistribution), [](float x) { return static_cast<int>(x); });
}