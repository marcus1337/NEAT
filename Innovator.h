
#include <map>
#include <utility> 

#ifndef INNOVATOR_H
#define INNOVATOR_H

class Innovator {
    typedef std::pair<int, int> par;
    typedef std::tuple<int,int,int> par3;

    Innovator();
    std::map<par, int> takenGeneIDs;
    std::map<par3, int> takenNodeIDs;

public:

    int geneNum, nodeNum;

    void reset();
    static Innovator& getInstance();

    int getNodeNum(int from, int to, int children);
    int getAnyNodeNum();
    int getMaxNodeNum();
    int getMaxEdgeNum();
    int getGeneNum(int from, int to);

    std::map<par, int> getAllGeneIDs();
    void setTakenGeneIDs(std::map<par, int> _takenGeneIDs);

    std::map<par3, int> getAllNodeIDs();
    void setTakenNodeIDs(std::map<par3, int> _takenNodeIDs);

    Innovator(Innovator const&) = delete;
    void operator=(Innovator const&) = delete;
};

#endif