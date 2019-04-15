#pragma once
#include <cstdlib>
#include <map>
#include <set>

#include <list> 
#include <stack> 
#include <vector>

class NEAT;

class Helper {
protected:

    static bool dfs(int current, std::set<int>& white, std::set<int>& gray, std::set<int>& black, NEAT& neat);
    static void move_vertex(int vertex, std::set<int>& source_set, std::set<int>& destination_set);

public:

    static float randf(float LO, float HI) {
        float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
        return r3;
    }
    static int randi(int LO, int HI) {
        int randNum = rand() % (HI - LO + 1) + LO;
        return randNum;
    }

    template<class K, class T>
    static bool mapContains(std::map<K,T>&m , K key) {
        if (m.find(key) == m.end()) return false;
        return true;
    }

    static bool hasLoop(NEAT& neat);
    static bool hasLoopEasy(NEAT& neat, int start);

    static void topologicalSortUtil(int v, std::vector<bool>& visited, std::stack<int>& topstack, NEAT& neat);

    static std::stack<int> topSort(NEAT& neat);

};