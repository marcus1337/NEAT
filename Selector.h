#include "NEAT.h"
#include "Specie.h"
#include <vector>
#include <utility>


#ifndef SELECTOR_H
#define SELECTOR_H

namespace NTE {
    class Selector {
        std::vector<float> getSpecieProbabilities(std::vector<Specie>& oldSpecies);
        std::vector<int> getDiscreteSpecieProbabilities(std::vector<Specie>& oldSpecies);
    public:
        int numSpeciesLimit = 200;
        int populationSize = 1;

        int getTotalAverageFitness(std::vector<Specie>& species);

        void removeStaleSpecies(std::vector<Specie>& species);
        void cullSpecies(std::vector<Specie>& species);

        std::vector<NEAT*> selectSingleIDs(std::vector<Specie>& species);
        std::vector<std::pair<NEAT*, NEAT*>> selectPairedIDs(std::vector<Specie>& species);
    };
}

#endif