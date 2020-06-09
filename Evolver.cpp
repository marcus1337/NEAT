#include "Evolver.h"

using namespace NTE;

Surprise Evolver::surprise;

void Evolver::makeNewGeneration(std::vector<NEAT>& neats, std::vector<NEAT>& oldNeats) {

    mapElites.storeElites(neats);
    mutater.modifyMutationRate(neats);

    speciator.speciate(neats);

    surprise.addSurpriseFitness(neats);
    speciator.sortSpecies(speciator.pool);

    selector.populationSize = neats.size();
    selector.cullSpecies(speciator.pool);
    breeder.selectedSingles = selector.selectSingleIDs(speciator.pool);
    breeder.selectedPairs = selector.selectPairedIDs(speciator.pool);

    breeder.newGeneration(neats, oldNeats, speciator.pool);

}