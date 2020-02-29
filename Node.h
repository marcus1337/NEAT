#pragma once
#include <set>
#include "Genome.h"

class Node {

    int nodTyp, id;

public:
    static const int INPUT = 1, HIDDEN = 2, OUTPUT = 3;
    std::set<Genome> genomes;
    float value = 0;

    int getType() const;
    int getID() const;

    Node();
    Node(int _id, int _nodTyp = HIDDEN);
    Node(const Node &n2);

    bool operator< (const Node &right) const;
    bool operator==(const Node& rhs) const;

protected:

};