#include "Mutate.h"
#include "NEAT.h"
#include "Innovator.h"
#include <iterator>
#include "Utils.h"
#include <vector>
#include <iostream>

typedef std::pair<int, int> par;

float Mutate::mutationrate = 0.02f;
float Mutate::mutationrateNewNode = 0.01;


bool Mutate::shouldMutate(float chance) {
    float r = Utils::randf(0.f, 100.f);
    if (r > chance)
        return false;
    return true;
}

bool Mutate::canAddGene(NEAT& neat, int from, int to) {
    int numIn = neat.numIn;
    int numOut = neat.numOut;
    return (from != to &&
        !Utils::isInNode(to, numIn)     && !Utils::isOutNode(from, numIn, numOut) &&
        !(Utils::isInNode(from, numIn)  &&  Utils::isInNode(to, numIn)) &&
        !neat.hasOppositeEdge(from, to) && !neat.hasOppositeEdge(to, from) &&
        !Utils::isCircle(neat.nodes, from, to));
}

void Mutate::linkMutate(NEAT& neat) {
    for (auto& a : neat.nodes)
    {
        for (auto& b : neat.nodes) //add edge a --> b
        {

            if (Utils::randf(0.f, 100.f) > mutationrate)
                continue;

            int from = a.second.getID();
            int to = b.second.getID();

            if (canAddGene(neat, from, to)) {
                neat.addGene(Genome(from, to));
            }
        }
    }
}

int Mutate::getUnusedNodeID(const std::map<int, Node>& nodes, int from, int to) {
    int numChildren = 0;
    int nodeID = Innovator::getInstance().getNodeNum(from, to, numChildren);
    while (Utils::mapContains(nodes, nodeID)) {
        nodeID = Innovator::getInstance().getNodeNum(from, to, ++numChildren);
    }
    return nodeID;
}


void Mutate::nodeMutate(NEAT& neat) {

    for (size_t i = 0; i < neat.gencopies.size(); i++) {

        if (!shouldMutate(mutationrateNewNode))
            continue;

        Genome gene = neat.gencopies[i];
        Node newNode(getUnusedNodeID(neat.nodes, gene.getFrom(), gene.getTo()));
        neat.nodes[newNode.getID()] = newNode;
        neat.addGene(Genome(gene.getFrom(), newNode.getID(), gene.enabled, gene.weight, 0));
        neat.addGene(Genome(newNode.getID(), gene.getTo(), gene.enabled, gene.weight, 0));
        neat.nodes[gene.getFrom()].genomes.erase(gene);
        neat.gencopies.erase(neat.gencopies.begin() + i);

    }
}


void Mutate::enableDisableMutate(NEAT& neat) {
    for (size_t i = 0; i < neat.gencopies.size(); i++) {
        if (!shouldMutate(mutationrate))
            continue;

        neat.gencopies[i].enabled = !neat.gencopies[i].enabled;
        neat.updateGene(neat.gencopies[i]);
    }
}

void Mutate::pointMutate(NEAT& neat) {
    for (size_t i = 0; i < neat.gencopies.size(); i++) {
        if (!shouldMutate(mutationrate))
            continue;

        if (shouldMutate(0.07f)) {
            neat.gencopies[i].weight = Utils::randf(-2.f, 2.f);
        }
        else {

            neat.gencopies[i].weight += Utils::randf(-0.3f, 0.3f);

            if (neat.gencopies[i].weight > 2.f)
                neat.gencopies[i].weight = 2.f;
            if (neat.gencopies[i].weight < -2.f)
                neat.gencopies[i].weight = -2.f;
        }

        neat.updateGene(neat.gencopies[i]);
    }
}


void Mutate::allMutations(NEAT& neat) {
    enableDisableMutate(neat);
    pointMutate(neat);
    linkMutate(neat);
    nodeMutate(neat);
}