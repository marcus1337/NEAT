#include "Evolver.h"

using namespace NTE;

void Evolver::makeNewGeneration(std::vector<NEAT>& neats, std::vector<NEAT>& oldNeats) {

    speciator.speciate(neats);
    breeder.newGeneration(neats, oldNeats, speciator.pool);
}