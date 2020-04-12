
#include "NEAT.h"
#include "Specie.h"
#include <future>

#ifndef SPECIATOR_H
#define SPECIATOR_H

class Speciator {
protected:

    int numOut, numIn, numChildrenLeft;

    void addRemainingGenesToNeat(NEAT& _neat, int fromIndex, std::vector<Genome>& genes);
    void addRemainingGenesToNeatRandomly(NEAT& _neat, int fromIndex, std::vector<Genome>& genes);
    void childFromEqualParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2);
    void childFromUnequalParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2);
    void incrementIDIndexes(int& i, int& j, int ID1, int ID2);

    void inheritNodesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2);
    void inheritGenesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2);

    int getChildIndex(int numKids);

public:

    int numAI = 1;
    int numSpeciesLimit = 200;
    static constexpr float c1 = 1.6f;
    static constexpr float c2 = 0.4f;
    static constexpr float c3 = 1.6f;
    static constexpr int crossChance = 75;
    int totAvg = 1;
    std::vector<Specie> pool;
    std::vector<NEAT>* children;
    int specieNum;
    int targetNumSpecies = 3;
    float speciateDelta = 3.0f;

    void adjustDynamicSpecieDelta();

    void init(int _numIn, int _numOut, int _numAI);
    void speciate(std::vector<NEAT>& neats, std::vector<NEAT>& oldNeats);
    void prepareForNewGeneration(std::vector<NEAT>& neats);
    void createSpecies(std::vector<NEAT>& neats);

    void preparePool();
    void sortSpecie(Specie& spec);

    virtual void fitnessSharing(std::vector<NEAT>& neats);
    void adjustFitnessShared(std::vector<NEAT>& neats, int index);
    int calcNumBreeds(const Specie& specie);
    int totalAvgFit();

    void newGeneration();
    virtual void newGeneration(std::vector<std::future<void>>& futures);

    void removeStaleSpecies();

    virtual void cullSpecies();
    bool isWeak(const Specie& specie, int numSpecies, int totalAverageFitness);
    void removeWeakSpecies();
    void removeWeaksInSpecies();
    void cullAllButOneFromSpecies();

    void addToSpecies(NEAT& neat);
    bool addToExistingSpecie(NEAT& neat);
    void addNewSpecie(NEAT& neat);

    void crossOver(NEAT& child, NEAT* n1, NEAT* n2);
    void breedChild(Specie& specie, int childIndex);
    void breedElite(int childIndex);

    void addGeneRandomly(NEAT& child, Genome gene1, Genome gene2);

    void breedFitnessBased(std::vector<std::future<void>>& futures, int numKids);
    void breedElitismOfSpecies(std::vector<std::future<void>>& futures, int numKids);

    bool sameSpecie(NEAT& n1, NEAT& n2);
    float weightDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);
    float disjointDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);
    float excessDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);
    bool IDInRange(int _id, std::vector<Genome>& genomeArr);

};

#endif