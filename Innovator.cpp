
#include "Innovator.h"
#include "Utils.h"
using namespace NTE;

Innovator::Innovator() : nodeNum(0) {}

void Innovator::reset() {
    takenNodeIDs.clear();
}

Innovator& Innovator::getInstance()
{
    static Innovator instance;
    return instance;
}

int Innovator::getNodeNum(int from, int to, int children) {
    par3 betweenEdges = std::make_tuple(from, to, children);
    mutex.lock();
    if (Utils::mapContains<par3>(takenNodeIDs, betweenEdges)) {
        int existingID = takenNodeIDs[betweenEdges];
        mutex.unlock();
        return existingID;
    }
    nodeNum++;
    takenNodeIDs[betweenEdges] = nodeNum;
    mutex.unlock();
    return nodeNum;
}

int Innovator::getGeneNum(int from, int to) {
    return Utils::elegantPair(from, to);
}

std::map<Innovator::par3, int> Innovator::getAllNodeIDs() {
    return takenNodeIDs;
}

void Innovator::setTakenNodeIDs(std::map<Innovator::par3, int> _takenNodeIDs) {
    takenNodeIDs = _takenNodeIDs;
}