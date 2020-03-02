
#include "NEAT.h"
#include "Specie.h"

#ifndef SPECIATOR_H
#define SPECIATOR_H

class Speciator {
    int numOut, numIn, numAI, numChildrenLeft;

    void addRemainingGenesToNeat(NEAT& _neat, int fromIndex, std::vector<Genome>& genes);
    void childFromEqualParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2);
    void childFromUnequalParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2);
    void incrementIDIndexes(int& i, int& j, int ID1, int ID2);
    void addLowestGeneOrRandom(NEAT& _neat, Genome& gene1, Genome& gene2);
    void inheritNodesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2);
    void inheritGenesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2);

public:

    static constexpr float c1 = 1.8f;
    static constexpr float c2 = 0.4f;
    static constexpr float crossChance = 0.75f;
    int totAvg = 1;
    std::vector<Specie> pool;
    std::vector<NEAT> children;
    int specieNum;

    Speciator();

    void init(int _numIn, int _numOut, int _numAI);
    void prepareForNewGeneration(std::vector<NEAT>& neats);
    void createSpecies(std::vector<NEAT>& neats);
    int calcNumBreeds(const Specie& specie);

    int totalAvgFit();
    void speciate(std::vector<NEAT>& neats);
    void crossOver(NEAT* n1, NEAT* n2);

    void breedChild(Specie& specie);
    bool isWeak(const Specie& o);
    void removeWeakSpecies();

    void newGeneration();
    void removeStaleSpecies();

    void sortPoolAndSpecies();
    void cullSpecies();
    void cullAllButOneFromSpecies();

    void addToSpecies(NEAT& neat);
    bool addToExistingSpecie(NEAT& neat);
    void addNewSpecie(NEAT& neat);

    void breedFitnessBased(int numKids);
    void breedElitismOfSpecies(int numKids);

    bool sameSpecie(NEAT& n1, NEAT& n2);
    float weightDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);
    float disjointDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);

};

#endif