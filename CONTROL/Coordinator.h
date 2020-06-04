#include "../NEAT.h"
#include "../Speciator.h"
#include <vector>
#include "../NEATDoubleBuffer.h"
#include "../Evolver.h"

#ifndef COORDINATOR_NEAT_H
#define COORDINATOR_NEAT_H

namespace NTE {

    class Coordinator {
        void initNEATBuffers(NEAT& neat, int numAI);

    public:

        NEATDoubleBuffer neatBuffer;
        Evolver evolver;
        int generation;

        void init(int numIn, int numOut, int numAI);
        void evolve();
        void calcInput(int index, float* inputs);
        float* getOutput(int index);
        void setFitness(int index, int fitness);
        void save(int index, std::string filename);
        void load(std::string filename, int numAI);

    };
}
#endif