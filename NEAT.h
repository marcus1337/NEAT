
#include <vector>
#include <map>
#include "Node.h"

#include <array>
#include <stack>

#ifndef NEAT_H
#define NEAT_H
namespace NTE {
    class NEAT {

        void storeOutput();
        void storeInput(std::vector<float> inputs);
        void propagateEdges(const Genome& genome, int nodeID);

        std::stack<int> preComputedTopStack;

    protected:
        void deleteUnusedNodes(); //Used for examining temporary NEAT

    public:
        bool isTopStackComputed = false;
        int maxGeneNum;

        int numIn, numOut;
        int fitness;

        std::map<int, Node> nodes;

        std::vector<Genome> gencopies;
        std::vector<float> outputs;

        void updateGene(Genome updatedGene);
        float sigmoid(float value);
        float reLu(float value);

        void calculateOutput(std::vector<float> inputs);
        void processNetworkData();
        void resetNodes();

        void copyPointer(const NEAT* np);
        NEAT();
        NEAT(const NEAT& _neat);
        NEAT(std::istream& stream);
        NEAT(int _numIn, int _numOut);
        void initBaseNodes();
        void initBaseGenes();

        void addGene(Genome gene);
        //void addRecurrentGene(Genome gene);

        bool hasOppositeEdge(int from, int to);
        bool hasEdge(int from, int to);
        int getNumGenes();
        int getNumHiddenNodes();

        std::vector<int> observedBehaviors = {0,0,0};
        //void resetRecurrentState();


        std::vector<int> getUsedNodeIDs();
        std::vector<Genome> getUsedGenomes();
    };
}
#endif