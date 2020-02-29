#include "Specie.h"

void Specie::calcAvgFit() {
    int total = 0;
    for (const auto& n : neats) {
        total += n->fitness;
    }
    averageFitness = total / neats.size();
}

Specie::Specie() : id(-1) {}
Specie::Specie(int _id) : id(_id) {}

bool Specie::operator < (const Specie &right) const
{
    return id < right.id;
}
bool Specie::operator==(const Specie& rhs) const { return this->id == rhs.id; }