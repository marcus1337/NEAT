#pragma once

#include <vector>
#include <map>
#include "Node.h"


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

    void topSortCalc(float* inputs);
    void resetNodes();

    void copyPointer(NEAT* np);
    NEAT();
    NEAT(std::istream& stream);
    NEAT(int _numIn, int _numOut);
    void initBaseNodes();

    void addGene(int from, int to, bool enabled, float weight, int childnodes);
    void addGene(int from, int to);
    void addGeneNoLoop(Genome gene);
    void removeRedundants();

};