
#include "Innovator.h"
#include "Utils.h"

Innovator::Innovator() : innovNum(0), innovNodeNum(0) {}

void Innovator::reset() {
    innovNum = innovNodeNum = 0;
    takenIDs.clear();
    takenMiddleIDs.clear();
}

Innovator& Innovator::getInstance()
{
    static Innovator instance;
    return instance;
}

int Innovator::getNewNodeNum(int from, int to, int children) {
    par3 betweenEdges = std::make_tuple(from, to, children);
    if (Utils::mapContains<par3>(takenMiddleIDs, betweenEdges)) {
        return takenMiddleIDs[betweenEdges];
    }
    innovNodeNum++;
    takenMiddleIDs[betweenEdges] = innovNodeNum;
    return innovNodeNum;
}

int Innovator::getAnyNodeNum() {
    innovNodeNum++;
    return innovNodeNum;
}

int Innovator::getMaxNodeNum() {
    return innovNodeNum;
}
int Innovator::getMaxEdgeNum() {
    return innovNum;
}

int Innovator::getNum(int from, int to) {
    par betweenNodes = std::make_pair(from, to);
    if (Utils::mapContains<par>(takenIDs, betweenNodes)) {
        return takenIDs[betweenNodes];
    }
    innovNum++;
    takenIDs[betweenNodes] = innovNum;
    return innovNum;
}