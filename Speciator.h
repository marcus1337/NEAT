
#include "NEAT.h"
#include "Specie.h"
#include <future>

#ifndef SPECIATOR_H
#define SPECIATOR_H
namespace NTE {
    class Speciator {
    protected:

        int numOut, numIn, numChildrenLeft;
        void incrementIDIndexes(int& i, int& j, int ID1, int ID2);

    public:

        int numAI = 1;
        static constexpr float c1 = 1.6f;
        static constexpr float c2 = 0.4f;
        static constexpr float c3 = 1.6f;
        std::vector<Specie> pool;
        int specieNum;
        int targetNumSpecies = 3;
        float speciateDelta = 3.0f;

        void adjustDynamicSpecieDelta();

        void init(int _numIn, int _numOut, int _numAI);
        void speciate(std::vector<NEAT>& neats);
        void prepareForNewGeneration(std::vector<NEAT>& neats);
        void createSpecies(std::vector<NEAT>& neats);

        void preparePool();
        void sortSpecie(Specie& spec);

        void fitnessSharing(std::vector<NEAT>& neats);
        void adjustFitnessShared(std::vector<NEAT>& neats, int index);

        void addToSpecies(NEAT& neat);
        bool addToExistingSpecie(NEAT& neat);
        void addNewSpecie(NEAT& neat);

        bool sameSpecie(NEAT& n1, NEAT& n2);
        float weightDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);
        float disjointDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);
        float excessDiff(std::vector<Genome>& g1, std::vector<Genome>& g2);
        bool IDInRange(int _id, std::vector<Genome>& genomeArr);

    };
}
#endif