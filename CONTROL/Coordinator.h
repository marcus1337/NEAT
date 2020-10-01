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
        std::vector<float> getOutput(int index);

        void calcEliteInput(int index, std::vector<float> inputs);
        std::vector<float> getEliteOutput(int index);

        void setFitness(int index, int fitness);
        void save(int index, std::string filename);
        void load(std::string filename, int numAI);

        void randomizePopulation(int minNodes, int maxNodes);
        void randomizePopulationFromElites();
        void mapElites();
        void saveElites(std::string foldername = "NEATS_ELITE");
        void loadElites(std::string foldername = "NEATS_ELITE");
        void storeElites();

        void setBehavior(int index, std::vector<int> behaviors);
        void setTargetSpecies(int numTargetSpecies);
        void setSurpriseEffect(float effect);
        void saveGeneration(std::string filename);
        void loadGeneration(std::string filename, int _generation);

        void setParentFoldername(std::string folderName);

        void setMaxNodes(int _maxNodes);
    };
}
#endif