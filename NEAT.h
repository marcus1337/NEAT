
#include <vector>
#include <map>
#include "Node.h"

#ifndef NEAT_H
#define NEAT_H

class NEAT {
public:
    int numIn, numOut;
    int fitness;

    std::map<int, Node> nodes;
    std::vector<Genome> gencopies;
    std::map<std::pair<int, int>, bool> busyEdges;
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

    void addGene(Genome gene);
    void addGeneNoLoop(Genome gene);
    void removeRedundants();

};

#endif