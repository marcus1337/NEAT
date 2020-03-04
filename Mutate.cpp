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


void Mutate::linkMutate(NEAT& neat) {
    for (auto& a : neat.nodes)
    {
        for (auto& b : neat.nodes) //add edge a --> b
        {

            if (Utils::randf(0.f, 100.f) > mutationrate)
                continue;
            Node& anode = a.second;
            Node& bnode = b.second;
            int from = anode.getID();
            int to = bnode.getID();

            if (from != to && !(bnode.getType() == Node::INPUT) && !(anode.getType() == Node::OUTPUT) &&
                !(anode.getType() == Node::INPUT && bnode.getType() == Node::INPUT) &&
                !Utils::mapContains<par, bool>(neat.busyEdges, std::make_pair(from, to))) {
                if (!Utils::isCircle(neat.nodes, from, to)) {
                    neat.addGene(Genome(from, to));
                }
                else {
                    neat.busyEdges[std::make_pair(from, to)] = true;
                    neat.busyEdges[std::make_pair(to, from)] = true;
                }
            }

        }
    }

}


void Mutate::nodeMutate(NEAT& neat) {

    for (size_t i = 0; i < neat.gencopies.size(); i++) {

        if (!shouldMutate(mutationrateNewNode))
            continue;

        neat.gencopies[i].enabled = false;
        neat.gencopies[i].childNodes++;
        neat.updateGene(neat.gencopies[i]);

        Node node(Innovator::getInstance().getNodeNum(neat.gencopies[i].getFrom(), neat.gencopies[i].getTo(), neat.gencopies[i].childNodes));

        neat.nodes[node.getID()] = node;
        neat.addGene(Genome(neat.gencopies[i].getFrom(), node.getID()));
        neat.addGene(Genome(node.getID(), neat.gencopies[i].getTo()));

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