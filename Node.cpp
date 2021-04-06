#include "Node.h"
using namespace NTE;
int Node::getType() const {
    return nodTyp;
}

int Node::getID() const {
    return id;
}

Node::Node() : nodTyp(0), id(0) {}
Node::Node(int _id, int _nodTyp) : id(_id), nodTyp(_nodTyp) {}
Node::Node(const Node &n2) : id(n2.id), nodTyp(n2.nodTyp), genomes(n2.genomes) {}

bool Node::operator< (const Node &right) const
{
    return id < right.id;
}
bool Node::operator==(const Node& rhs) const { return this->id == rhs.id; }


void Node::removeDisabledGenes() {
    for (auto it = genomes.begin(); it != genomes.end(); ) {
        if ((*it).enabled == false) {
            it = genomes.erase(it);
        }
        else {
            ++it;
        }
    }
}