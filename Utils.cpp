#include "Utils.h"

#include <cstdlib>
#include <random>
#include <functional> 
#include <list>
#include "Innovator.h"

bool Utils::isCircle(std::map<int, Node>& nodes, int from, int to) {
    bool result = false;
    Genome gene(from, to); //ATM genes.size() may not reflect innocationNumber for genes
    nodes[from].genomes.insert(gene);

    if (Utils::hasLoopEasy(nodes, to)) {
        result = true;
    }

    nodes[from].genomes.erase(gene);
    return result;
}

bool Utils::hasLoop(std::map<int, Node>& nodes) {
    std::set<int> white;
    std::set<int> grey;
    std::set<int> black;

    for (size_t i = 0; i < nodes.size(); i++) {
        white.insert(nodes[i].getID());
    }

    while (!white.empty()) {
        auto it = white.begin();
        int current = *it;
        if (dfs(current, white, grey, black, nodes)) {
            return true;
        }
    }

    return false;
}

bool Utils::hasLoopEasy(std::map<int, Node>& neat, int start) {
    std::set<int> white;
    std::set<int> grey;
    std::set<int> black;
    white.insert(start);

    while (!white.empty()) {
        auto it = white.begin();
        int current = *it;
        if (dfs(current, white, grey, black, neat)) {
            return true;
        }
    }

    return false;
}

bool Utils::dfs(int current, std::set<int>& white, std::set<int>& gray, std::set<int>& black, std::map<int, Node>& nodes) {
    move_vertex(current, white, gray);
    for (const Genome& genome : nodes[current].genomes) {
        if (black.find(genome.getTo()) != black.end()) {
            continue;
        }
        if (gray.find(genome.getTo()) != gray.end()) {
            return true;
        }
        if (dfs(genome.getTo(), white, gray, black, nodes)) {
            return true;
        }
    }

    move_vertex(current, gray, black);
    return false;
}


void Utils::move_vertex(int vertex, std::set<int>& source_set, std::set<int>& destination_set) {
    source_set.erase(vertex);
    destination_set.insert(vertex);
}


void Utils::topologicalSortUtil(int v, std::vector<bool>& visited, std::stack<int>& topstack, std::map<int, Node>& nodes) {
    visited[v] = true;
    std::list<int>::iterator i;

    for (const Genome& genome : nodes[v].genomes) {

        if (!visited[genome.getTo()])
            topologicalSortUtil(genome.getTo(), visited, topstack, nodes);
    }

    topstack.push(v);
}

std::stack<int> Utils::topSort(std::map<int, Node>& nodes) {
    std::stack<int> topstack;
    std::vector<bool> visited(Innovator::getInstance().innovNodeNum+1, false);

    for (const auto& n : nodes)
    {
        int id = n.second.getID();
        if(!visited[id]) {
            topologicalSortUtil(id, visited, topstack, nodes);
        }
    }
    return topstack;
}


bool Utils::randomBool() {
    static auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
    return gen();
}

float Utils::randf(float LO, float HI) {
    float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
    return r3;
}
int Utils::randi(int LO, int HI) {
    int randNum = rand() % (HI - LO + 1) + LO;
    return randNum;
}