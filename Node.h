
#include <set>
#include "Genome.h"

#ifndef NODE_H
#define NODE_H
namespace NTE {
    class Node {

        int nodTyp, id;

    public:
        static const int INPUT = 1, HIDDEN = 2, OUTPUT = 3;
        mutable std::set<Genome> genomes, recurrentGenomes;
        mutable float value = 0;
        mutable float recurrentValue = 0;

        int getType() const;
        int getID() const;

        Node();
        Node(int _id, int _nodTyp = HIDDEN);
        Node(const Node &n2);

        bool operator< (const Node &right) const;
        bool operator==(const Node& rhs) const;

    protected:

    };
}
#endif