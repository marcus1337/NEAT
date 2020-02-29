
#include <map>
#include <utility> 

#ifndef INNOVATOR_H
#define INNOVATOR_H

class Innovator {
    typedef std::pair<int, int> par;
    typedef std::tuple<int,int,int> par3;

    Innovator();
    std::map<par, int> takenIDs;
    std::map<par3, int> takenMiddleIDs;

public:

    int innovNum, innovNodeNum;

    void reset();
    static Innovator& getInstance();

    int getNewNodeNum(int from, int to, int children);
    int getAnyNodeNum();
    int getMaxNodeNum();
    int getMaxEdgeNum();
    int getNum(int from, int to);

    Innovator(Innovator const&) = delete;
    void operator=(Innovator const&) = delete;

};

#endif