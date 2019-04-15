#pragma once
#include <vector>
#include <map>
#include <tuple>
#include "Node.h"
#include "Innovator.h"
#include "Mutate.h"
#include <set>

#include <iostream>
#include <functional>
#include <list>

class NEAT {

    int numIn, numOut;

    void calc(int nowID, float value) {

        float res = 0;
        Node& n = nodes[nowID];
        if (n.getType() == Node::OUTPUT) {
            outputs[nowID - numIn] += value;
        }
        else {
            for (Genome genome : n.genomes) {
                calc(genome.getTo(), value*genome.weight);
            }
        }
    }

public:

    bool hasLoop() {
        return Helper::hasLoop(*this);
    }



    std::map<int, Node> nodes;
    std::vector<float> outputs;
    std::vector<Genome> gencopies;
    std::map<std::pair<int,int>, bool> busyEdges;

    void updateGene(Genome updatedGene) { 
        Node& nowNode = nodes[updatedGene.getFrom()];
        auto found = nowNode.genomes.find(updatedGene);
        nowNode.genomes.insert(updatedGene);
    }

    void calcOut(float* inputs) {
        for (int i = 0; i < numIn; i++) {
            calc(i, inputs[i]);
        }
    }

    void reset() {
        for (size_t i = 0; i < outputs.size(); i++)
            outputs[i] = 0;
    }

    NEAT(int _numIn, int _numOut) : numIn(_numIn), numOut(_numOut), outputs(_numOut)  {
        for (int i = 0; i < numIn; i++) {
            Node node(i, Node::INPUT);
            nodes[i] = node;
        }

        for (int i = 0; i < numOut; i++) {
            Node node(i+numIn, Node::OUTPUT);
            nodes[i + numIn] = node;
        }

        for (int i = 0; i < numIn; i++) {
            for (int j = numIn; j < numIn + numOut; j++) {
                int from = nodes[i].getID();
                int to = nodes[j].getID();
                addGene(from, to);
            }
        }

    }

    
    void addGene(int from, int to) {
        Genome gene(from, to);
        nodes[from].genomes.insert(gene);
        gencopies.push_back(gene);
        busyEdges[std::make_pair(from, to)] = true;
        busyEdges[std::make_pair(to, from)] = true;
    }


};