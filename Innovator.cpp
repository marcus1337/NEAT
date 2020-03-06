
#include "Innovator.h"
#include "Utils.h"

Innovator::Innovator() : geneNum(0), nodeNum(0) {}

void Innovator::reset() {
    geneNum = nodeNum = 0;
    takenGeneIDs.clear();
    takenNodeIDs.clear();
}

Innovator& Innovator::getInstance()
{
    static Innovator instance;
    return instance;
}

int Innovator::getNodeNum(int from, int to, int children) {
    par3 betweenEdges = std::make_tuple(from, to, children);
    if (Utils::mapContains<par3>(takenNodeIDs, betweenEdges))
        return takenNodeIDs[betweenEdges];
    nodeNum++;
    takenNodeIDs[betweenEdges] = nodeNum;
    return nodeNum;
}

int Innovator::getGeneNum(int from, int to) {
    par betweenNodes = std::make_pair(from, to);
    if (Utils::mapContains<par>(takenGeneIDs, betweenNodes))
        return takenGeneIDs[betweenNodes];
    geneNum++;
    takenGeneIDs[betweenNodes] = geneNum;
    return geneNum;
}

int Innovator::getAnyNodeNum() {
    nodeNum++;
    return nodeNum;
}

int Innovator::getMaxNodeNum() {
    return nodeNum;
}
int Innovator::getMaxEdgeNum() {
    return geneNum;
}

std::map<Innovator::par3, int> Innovator::getAllNodeIDs() {
    return takenNodeIDs;
}

void Innovator::setTakenNodeIDs(std::map<Innovator::par3, int> _takenNodeIDs) {
    takenNodeIDs = _takenNodeIDs;
}

std::map<Innovator::par, int> Innovator::getAllGeneIDs() {
    return takenGeneIDs;
}

void Innovator::setTakenGeneIDs(std::map<par, int> _takenGeneIDs) {
    takenGeneIDs = _takenGeneIDs;
}