#include "StandardStrategy.h"

#include <iostream>

using namespace std;

void StandardStrategy::fitnessSharing(std::vector<NEAT>& neats) {
    for (int i = 0; i < numAI; i++)
        adjustFitnessShared(neats, i);
}

void StandardStrategy::newGeneration(std::vector<std::future<void>>& futures) {
    breedFitnessBased(futures, numChildrenLeft / 2);
    numChildrenLeft -= numChildrenLeft / 2;
    breedElitismOfSpecies(futures, numChildrenLeft);
}

void StandardStrategy::cullSpecies() {
    removeStaleSpecies();
    removeWeaksInSpecies();
    removeWeakSpecies();
}
