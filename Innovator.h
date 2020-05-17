
#include <map>
#include <utility> 
#include "MiniMutex.h"

#ifndef INNOVATOR_H
#define INNOVATOR_H
namespace NTE {
    class Innovator {
        typedef std::pair<int, int> par;
        typedef std::tuple<int, int, int> par3;

        Innovator();
        std::map<par3, int> takenNodeIDs;
        MiniMutex mutex;

    public:

        int nodeNum;

        void reset();
        static Innovator& getInstance();

        int getNodeNum(int from, int to, int children);
        int getGeneNum(int from, int to);

        std::map<par3, int> getAllNodeIDs();
        void setTakenNodeIDs(std::map<par3, int> _takenNodeIDs);

        Innovator(Innovator const&) = delete;
        void operator=(Innovator const&) = delete;
    };
}
#endif