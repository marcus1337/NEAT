#include "NEAT.h"

#include <set>
#include <iostream>
#include <math.h>
#include "Utils.h"
#include <algorithm>

using namespace NTE;

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

void NEAT::calculateOutput(std::vector<float> inputs) {
    storeInput(inputs);
    //propagateRecurrentEdges();
    processNetworkData();
    storeOutput();
    resetNodes();
}

void NTE::NEAT::processNetworkData()
{
    std::stack<int> topstack = Utils::topSort(nodes);
    while (!topstack.empty())
    {
        int nowID = topstack.top();
        topstack.pop();
        for (const Genome& genome : nodes[nowID].genomes)
            propagateEdges(genome, nowID);
    }
}

/*void NTE::NEAT::propagateRecurrentEdges()
{
    for (auto& node : nodes)
        for (const Genome& genome : node.second.recurrentGenomes) {
            if (!genome.enabled)
                continue;
            int to = genome.getTo();
            nodes[to].value += node.second.recurrentValue*genome.weight;
        }
}*/

void NEAT::propagateEdges(const Genome& genome, int nodeID) {
    if (!genome.enabled)
        return;
    int to = genome.getTo();
    if (nodes[nodeID].getType() != Node::INPUT)
        nodes[nodeID].value = sigmoidNN(nodes[nodeID].value);
    //nodes[nodeID].recurrentValue = nodes[nodeID].value;
    nodes[to].value += nodes[nodeID].value*genome.weight;
}

void NEAT::storeInput(std::vector<float> inputs) {
    for (int i = 0; i < numIn; i++)
        nodes[i].value = inputs[i];
}

void NEAT::storeOutput() {
    outputs = std::vector<float>(numOut, 0);
    for (int i = numIn; i < numIn + numOut; i++) {
        outputs[i - numIn] = nodes[i].value;
        outputs[i - numIn] = sigmoidNN(outputs[i - numIn]);
    }
}

void NEAT::resetNodes() {
    for (auto& n : nodes)
        n.second.value = 0;
}

void NEAT::copyPointer(const NEAT* np) {
    if (np == nullptr)
        return;
    nodes = np->nodes;
    gencopies = np->gencopies;
    numIn = np->numIn;
    numOut = np->numOut;
    fitness = np->fitness;
}

NEAT::NEAT(const NEAT& _neat) {
    copyPointer(&_neat);
}

NEAT::NEAT() :numIn(-1), numOut(-1) {}

NEAT::NEAT(std::istream& stream) {

    for (size_t i = 0; i < 3; i++)
        stream >> observedBehaviors[i];

    stream >> numIn >> numOut >> fitness;
    int numGenes, numRecurrentGenes;
    stream >> numGenes >> numRecurrentGenes;
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

  /*  for (int i = 0; i < numRecurrentGenes; i++) {
        int from, to;
        bool enabled;
        float weight;
        stream >> from >> to >> enabled >> weight;
        Genome gene(from, to);
        gene.enabled = enabled;
        gene.weight = weight;
        addRecurrentGene(gene);
    }*/
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

void NEAT::initBaseGenes() {
    for (int i = 0; i < numIn; i++) {
        for (int j = numIn; j < numIn + numOut; j++) {
            int from = nodes[i].getID();
            int to = nodes[j].getID();
            addGene(Genome(from, to));
        }
    }
}

NEAT::NEAT(int _numIn, int _numOut) : numIn(_numIn), numOut(_numOut), fitness(0) {
    initBaseNodes();
    initBaseGenes();
}

/*void NEAT::addRecurrentGene(Genome gene) {
    nodes[gene.getFrom()].recurrentGenomes.insert(gene);
    recurrentGeneCopies.push_back(gene);
}*/

void NEAT::addGene(Genome gene) {
    if (!Utils::mapContains<int, Node>(nodes, gene.getFrom()))
        nodes[gene.getFrom()] = Node(gene.getFrom());
    if (!Utils::mapContains<int, Node>(nodes, gene.getTo()))
        nodes[gene.getTo()] = Node(gene.getTo());

    nodes[gene.getFrom()].genomes.insert(gene);
    gencopies.push_back(gene);
}

int NEAT::getNumGenes() {
    int result = 0;
    for (const auto&[key, node] : nodes) {
        result += node.genomes.size();
    }
    return result;
}

bool NEAT::hasOppositeEdge(int from, int to) {
    if (Utils::mapContains(nodes, to)) {
        const auto& container = nodes[to].genomes;
        return container.find(Genome::dummyGenome(to, from)) != container.end();
    }
    return false;
}

bool NEAT::hasEdge(int from, int to) {
    return hasOppositeEdge(to, from);
}

int NEAT::getNumHiddenNodes() {
    return nodes.size() - numIn - numOut;
}


/*void NEAT::resetRecurrentState() {
    for (auto& node : nodes)
        node.second.recurrentValue = 0.f;
}*/