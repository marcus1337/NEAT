#include "NEAT.h"

#include <set>
#include <iostream>
#include <math.h>
#include "Utils.h"
#include <algorithm>

void NEAT::updateGene(Genome updatedGene) {
    Node& nowNode = nodes[updatedGene.getFrom()];
    nowNode.genomes.insert(updatedGene);
}

float NEAT::sigmoidNN(float value) {
    return (1.0f / (1.0f + exp2f(-value)));
}

float NEAT::reLu(float value) {
    return std::max<float>(0.0f, value);
}

void NEAT::calculateOutput(float* inputs) {
    for (int i = 0; i < numIn; i++) {
        nodes[i].value = inputs[i];
    }

    std::stack<int> topstack = Utils::topSort(nodes);

    while (!topstack.empty())
    {
        int nowID = topstack.top();
        topstack.pop();

        for (const Genome& genome : nodes[nowID].genomes) {
            if (!genome.enabled)
                continue;
            int to = genome.getTo();
            if (nodes[nowID].getType() != Node::INPUT) { //Activation functions, hidden layers
                nodes[nowID].value = reLu(nodes[nowID].value);
            }
            nodes[to].value += nodes[nowID].value*genome.weight;
        }
    }

    for (int i = numIn; i < numIn + numOut; i++) {
        outputs[i - numIn] = nodes[i].value;
        outputs[i - numIn] = sigmoidNN(outputs[i - numIn]);
    }
    resetNodes();
}

void NEAT::resetNodes() {
    for (auto& n : nodes)
    {
        n.second.value = 0;
    }
}

void NEAT::copyPointer(const NEAT* np) {
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

NEAT::NEAT(const NEAT& _neat) {
    copyPointer(&_neat);
}

NEAT::NEAT() :numIn(-1), numOut(-1) {}

NEAT::NEAT(std::istream& stream) {
    stream >> numIn >> numOut >> fitness;
    outputs.resize(numOut);
    int numGenes;
    stream >> numGenes;
    initBaseNodes();

    for (int i = 0; i < numGenes; i++) {
        int from, to, childnodes;
        bool enabled;
        float weight;
        stream >> from >> to >> enabled >> weight >> childnodes;

        if (!Utils::mapContains<int, Node>(nodes, from)) {
            nodes[from] = Node(from);
        }
        if (!Utils::mapContains<int, Node>(nodes, to)) {
            nodes[to] = Node(to);
        }
        addGene(Genome(from, to, enabled, weight, childnodes));
    }
}

void NEAT::initBaseNodes() {
    for (int i = 0; i < numIn; i++) {
        Node node(i, Node::INPUT);
        nodes[i] = node;
    }

    for (int i = 0; i < numOut; i++) {
        Node node(i + numIn, Node::OUTPUT);
        nodes[i + numIn] = node;
    }
}


NEAT::NEAT(int _numIn, int _numOut) : numIn(_numIn), numOut(_numOut), fitness(0), outputs(_numOut) {
    initBaseNodes();

    for (int i = 0; i < numIn; i++) {
        for (int j = numIn; j < numIn + numOut; j++) {
            int from = nodes[i].getID();
            int to = nodes[j].getID();
            addGene(Genome(from, to));
        }
    }
}

void NEAT::addGene(Genome gene) {
    nodes[gene.getFrom()].genomes.insert(gene);
    gencopies.push_back(gene);
    busyEdges[std::make_pair(gene.getFrom(), gene.getTo())] = true;
    busyEdges[std::make_pair(gene.getFrom(), gene.getTo())] = true;
}

void NEAT::addGeneNoLoop(Genome gene) { //can cause loops, should be excess gene before add
    int from = gene.getFrom();
    int to = gene.getTo();
    if (Utils::mapContains<std::pair<int, int>, bool>(busyEdges, std::make_pair(from, to))) {
        return;
    }

    if (!Utils::mapContains<int, Node>(nodes, from)) {
        nodes[from] = Node(from);
    }
    if (!Utils::mapContains<int, Node>(nodes, to)) {
        nodes[to] = Node(to); 
    }
    if (!Utils::isCircle(nodes, from, to)) {
        addGene(gene);
    }
}

void NEAT::removeRedundants() { //removes edges and nodes with dead ends
    std::set<int> badFroms;
    std::set<int> toBeRemoved;
    std::vector<Genome> savedCopies;

    for (const auto& x : nodes) {
        if (x.second.genomes.size() == 0 && x.second.getType() == Node::HIDDEN) {
            badFroms.insert(x.second.getID());
        }
    }

    for (int i = 0; i < gencopies.size(); i++) {
        int from = gencopies[i].getFrom();
        int to = gencopies[i].getTo();
        if (badFroms.find(to) != badFroms.end()) {
            toBeRemoved.insert(i);
            nodes[from].genomes.erase(gencopies[i]);
        }
    }

    for (int x : badFroms) {
        nodes.erase(x);
    }

    savedCopies.reserve(gencopies.size());
    for (int i = 0; i < gencopies.size(); i++) {
        bool is_in = toBeRemoved.find(i) != toBeRemoved.end();
        if (!is_in) {
            savedCopies.push_back(gencopies[i]);
        }
    }

    gencopies = savedCopies;

}