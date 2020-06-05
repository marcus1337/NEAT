
#include "Specie.h"
#include "NEAT.h"
#include <future>

#ifndef BREEDER_H
#define BREEDER_H

namespace NTE {
    class Breeder {

        void addRemainingGenesToNeat(NEAT& _neat, int fromIndex, std::vector<Genome>& genes);
        void childFromUnequalParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2);
        void incrementIDIndexes(int& i, int& j, int ID1, int ID2);
        void inheritNodesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2);
        void inheritGenesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2);
        int getChildIndex(int numKids);

    public:
        int specieNum = 1;
        int numIn, numOut, numAI;

        std::vector<NEAT>* children;

        std::vector<Specie>* pool;
        int numChildrenLeft;

        void newGeneration(std::vector<NEAT>& neats, std::vector<NEAT>& oldNeats, std::vector<Specie>& _pool);
        void newGeneration(std::vector<std::future<void>>& futures);

        void crossOver(NEAT& child, NEAT* n1, NEAT* n2);
        void breedChild(NEAT* g1, NEAT* g2, int childIndex);
        void breedElite(int childIndex, NEAT* neat);
        void addGeneRandomly(NEAT& child, Genome gene1, Genome gene2);
        void breedFitnessBased(std::vector<std::future<void>>& futures, int numKids);
        void breedElitismOfSpecies(std::vector<std::future<void>>& futures, int numKids);


        std::vector<std::pair<NEAT*, NEAT*>> selectedPairs;
        std::vector<NEAT*> selectedSingles;
    };
}

#endif // !BREEDER_H