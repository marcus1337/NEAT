#include "Specie.h"
#include "Utils.h"

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