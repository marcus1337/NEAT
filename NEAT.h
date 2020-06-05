
#include <vector>
#include <map>
#include "Node.h"

#include <array>

#ifndef NEAT_H
#define NEAT_H
namespace NTE {
    class NEAT {

        void storeOutput();
        void storeInput(float* inputs);
        void propagateEdge(const Genome& genome, int nodeID);

    public:
        int maxGeneNum;

        int numIn, numOut;
        int fitness;

        std::map<int, Node> nodes;

        std::vector<Genome> gencopies;
        std::vector<float> outputs;

        void updateGene(Genome updatedGene);
        float sigmoidNN(float value);
        float reLu(float value);

        void calculateOutput(float* inputs);
        void resetNodes();

        void copyPointer(const NEAT* np);
        NEAT();
        NEAT(const NEAT& _neat);
        NEAT(std::istream& stream);
        NEAT(int _numIn, int _numOut);
        void initBaseNodes();
        void initBaseGenes();

        void addGene(Genome gene);

        bool hasOppositeEdge(int from, int to);
        bool hasEdge(int from, int to);
        int getNumGenes();

        std::vector<int> observedBehaviors = {0,0,0};
    };
}
#endif