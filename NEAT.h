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

public:

    bool hasLoop() {
        return Helper::hasLoop(*this);
    }
    int fitness;

    std::map<int, Node> nodes;
    std::vector<Genome> gencopies;
    std::map<std::pair<int,int>, bool> busyEdges;
    std::vector<float> outputs;

    void updateGene(Genome updatedGene) { 
        Node& nowNode = nodes[updatedGene.getFrom()];
        auto found = nowNode.genomes.find(updatedGene);
        nowNode.genomes.insert(updatedGene);
    }

    void topSortCalc(float* inputs) {
        for (int i = 0; i < numIn; i++) {
            nodes[i].value = inputs[i];
        }

        std::stack<int> topstack = Helper::topSort(*this);
        while(!topstack.empty())
        {
            int nowID = topstack.top();
            topstack.pop();

            for (const Genome& genome : nodes[nowID].genomes) {
                if (!genome.enabled)
                    continue;
                int to = genome.getTo();
                nodes[to].value += nodes[nowID].value*genome.weight;
            }
        }

        for (int i = numIn; i < numIn + numOut; i++) {
            outputs[i - numIn] = nodes[i].value;
        }
        reset();
    }

    void reset() {
        for (auto& n : nodes)
        {
            n.second.value = 0;
        }
    }

    void copyPointer(NEAT* np) {
        if (np == nullptr)
            return;
        nodes = np->nodes;
        gencopies = np->gencopies;
        busyEdges = np->busyEdges;
        outputs = np->outputs;
        numIn = np->numIn;
        numOut = np->numOut;
        fitness = np->fitness;
    }

    NEAT() :numIn(-1), numOut(-1) {}

    NEAT(int _numIn, int _numOut) : numIn(_numIn), numOut(_numOut), fitness(0), outputs(_numOut) {
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