#include "../NEAT.h"
#include "../Speciator.h"
#include <vector>
#include "../NEATDoubleBuffer.h"
#include "../Evolver.h"
#include "IOstuff.h"

#ifndef COORDINATOR_NEAT_H
#define COORDINATOR_NEAT_H

namespace NTE {

    class Coordinator {
        void initNEATBuffers(NEAT& neat, int numAI);

    public:

        NEATDoubleBuffer neatBuffer;
        static Evolver evolver;
        int generation;
        static IOstuff ioStuff;

        void init(int numIn, int numOut, int numAI);
        void evolve();
        void calcInput(int index, std::vector<float> inputs);
        //void resetRecurrentState(int index);
        std::vector<float> getOutput(int index);

        void calcEliteInput(int index, std::vector<float> inputs);
        std::vector<float> getEliteOutput(int index);
        std::vector<NEAT*> eliteVector;
        void storeElitesInVector();
        void changeEliteFitnessAndBehvaior(int index, int newFitness, std::vector<int> behaviors);
        void refactorEliteMapping();

        void setFitness(int index, int fitness);
        void save(int index, std::string filename);
        void load(std::string filename, int numAI);

        void randomizePopulation(int minNodes, int maxNodes);
        void randomizePopulationFromElites();
        void mapElites();

        void loadBestElite(std::string fileName, std::string foldername = "NEATS_ELITE_BEST");
        void saveBestElite(std::string fileName, std::string foldername = "NEATS_ELITE_BEST");

        void saveElites(std::string foldername = "NEATS_ELITE");
        void loadElites(std::string foldername = "NEATS_ELITE");
        void storeElites();

        void insertEliteIntoGeneration(int eliteIndex, int aiIndex);

        void setBehavior(int index, std::vector<int> behaviors);
        void setTargetSpecies(int numTargetSpecies);
        void setSurpriseEffect(float effect);
        void saveGeneration(std::string filename);
        void loadGeneration(std::string filename, int _generation);

        void setParentFoldername(std::string folderName);

        void setMaxHiddenNodes(int _maxNodes);

        int getNumElites();
        int getNumElitesOfUniqueDimensionValue(int dimension);
        int getNumElitesOfDimensionWithValue(int dimension, int value);
        int getNumElitesOfDimensionWithinThreshold(int dimension, int low, int high);
        std::vector<int> getBestEliteBehavior();
        void setMutationRates(float newNodeRate, float newLinkRate, float randomizeLinkRate, float mutateLinkRate, float enableDisableLinkRate, bool enableExtraMutationRate = false);
    
    };
}
#endif