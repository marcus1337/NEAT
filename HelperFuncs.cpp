#include "HelperFuncs.h"
#include "NEAT.h"
#include <iostream>

bool Helper::hasLoop(NEAT& neat) {
    std::set<int> white;
    std::set<int> grey;
    std::set<int> black;

    for (int i = 0; i < neat.nodes.size(); i++) {
        white.insert(neat.nodes[i].getID());
    }

    while (!white.empty()) {
        auto it = white.begin();
        int current = *it;
        if (dfs(current, white, grey, black, neat)) {
            return true;
        }
    }

    return false;
}


bool Helper::dfs(int current, std::set<int>& white, std::set<int>& gray, std::set<int>& black, NEAT& neat) {
    move_vertex(current, white, gray);
    for (const Genome& genome : neat.nodes[current].genomes) {
        if (black.find(genome.getTo()) != black.end()) {
            continue;
        }
        if (gray.find(genome.getTo()) != gray.end()) {
            return true;
        }
        if (dfs(genome.getTo(), white, gray, black, neat)) {
            return true;
        }
    }

    move_vertex(current, gray, black);
    return false;
}


void Helper::move_vertex(int vertex, std::set<int>& source_set, std::set<int>& destination_set) {
    source_set.erase(vertex);
    destination_set.insert(vertex);
}