#include "Mutate.h"
#include "NEAT.h"
#include "Innovator.h"
#include <iterator>
#include "Utils.h"
#include <vector>
#include <iostream>
using namespace NTE;
typedef std::pair<int, int> par;

float Mutate::mutationrate = 2.f;

float Mutate::newNodeRate = 1.f;
float Mutate::newLinkRate = 1.f;

float Mutate::enableDisableLinkRate = 1.f;
float Mutate::randomizeLinkRate = 7.f;
float Mutate::mutateLinkRate = 1.f;


bool Mutate::shouldMutate(float chance) {
    float r = Utils::randf(0.f, 1000.f);
    if (r > chance)
        return false;
    return true;
}

bool Mutate::canAddGene(NEAT& neat, int from, int to) {
    int numIn = neat.numIn;
    int numOut = neat.numOut;
    return (from != to &&
        !Utils::isInNode(to, numIn) && !Utils::isOutNode(from, numIn, numOut) &&
        !(Utils::isInNode(from, numIn) && Utils::isInNode(to, numIn)) &&
        !neat.hasOppositeEdge(from, to) && !neat.hasOppositeEdge(to, from) &&
        !Utils::isCircle(neat.nodes, from, to));
}

void Mutate::linkMutate(NEAT& neat) {

    if (!shouldMutate(newLinkRate))
        return;

    std::vector<std::pair<int, int>> possibleNewLinks;

    for (auto& a : neat.nodes)
    {
        for (auto& b : neat.nodes) //add edge a --> b
        {
            int from = a.second.getID();
            int to = b.second.getID();

            if (canAddGene(neat, from, to))
                possibleNewLinks.push_back(std::make_pair(from, to));
        }
    }

    if (possibleNewLinks.empty())
        return;

    int randCrossIndex = Utils::randi(0, possibleNewLinks.size() - 1);
    std::pair<int, int> crossPair = possibleNewLinks[randCrossIndex];
    neat.addGene(Genome(crossPair.first, crossPair.second));
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

    if (!shouldMutate(newNodeRate))
        return;
    int randIndex = Utils::randi(0, neat.gencopies.size() - 1);

    Genome gene = neat.gencopies[randIndex];
    Node newNode(getUnusedNodeID(neat.nodes, gene.getFrom(), gene.getTo()));
    neat.nodes[newNode.getID()] = newNode;
    neat.addGene(Genome(gene.getFrom(), newNode.getID(), gene.enabled, gene.weight, 0));
    neat.addGene(Genome(newNode.getID(), gene.getTo(), gene.enabled, gene.weight, 0));
    neat.nodes[gene.getFrom()].genomes.erase(gene);
    neat.gencopies.erase(neat.gencopies.begin() + randIndex);
}


void Mutate::enableDisableMutate(NEAT& neat) {

    if (!shouldMutate(enableDisableLinkRate))
        return;
    int randIndex = Utils::randi(0, neat.gencopies.size() - 1);

    neat.gencopies[randIndex].enabled = !neat.gencopies[randIndex].enabled;
    neat.updateGene(neat.gencopies[randIndex]);
}

void Mutate::pointMutate(NEAT& neat) {

    int randIndex = Utils::randi(0, neat.gencopies.size() - 1);

    if (shouldMutate(randomizeLinkRate)) {
        neat.gencopies[randIndex].weight = Utils::randf(-2.f, 2.f);
    }
    else {
        neat.gencopies[randIndex].weight += Utils::randf(-0.3f, 0.3f);
        if (neat.gencopies[randIndex].weight > 2.f)
            neat.gencopies[randIndex].weight = 2.f;
        if (neat.gencopies[randIndex].weight < -2.f)
            neat.gencopies[randIndex].weight = -2.f;
    }

    neat.updateGene(neat.gencopies[randIndex]);
}


void Mutate::allMutations(NEAT& neat) {
    enableDisableMutate(neat);
    pointMutate(neat);

    if (shouldMutate(mutateLinkRate))
        linkMutate(neat);
    nodeMutate(neat);
}