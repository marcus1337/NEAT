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

        std::vector<NEAT>& getNEATsRef();

        NEATDoubleBuffer neatBuffer;
        Evolver evolver;
        int generation;
        IOstuff ioStuff;

        void init(int numIn, int numOut, int numAI);
        void evolve();
        void calcInput(int index, float* inputs);
        float* getOutput(int index);
        void setFitness(int index, int fitness);
        void save(int index, std::string filename);
        void load(std::string filename, int numAI);

        void randomizePopulation(int minNodes, int maxNodes);
        void randomizePopulationFromElites();
        void mapElites();
        void saveElites(std::string foldername = "TREES_ELITE");
        void loadElites(std::string foldername = "TREES_ELITE");
        void storeElites();

        void setBehavior(int index, std::vector<int> behaviors);
        void setTargetSpecies(int numTargetSpecies);
        void setSurpriseEffect(float effect);
        void saveGeneration(std::string filename);
        void loadGeneration(std::string filename, int _generation);
    };
}
#endif