
#include <stack> 
#include <map>
#include <set>
#include <vector>

#include "Node.h"

#ifndef UTILS_H
#define UTILS_H

class Utils {
protected:

    static bool dfs(int current, std::set<int>& white, std::set<int>& gray, std::set<int>& black, std::map<int, Node>& nodes);
    static void move_vertex(int vertex, std::set<int>& source_set, std::set<int>& destination_set);

public:

    static bool randomBool();
    static float randf(float LO, float HI);
    static int randi(int LO, int HI);

    template <class T> static void swap(T& a, T& b)
    {
        T c(a); a = b; b = c;
    }

    template<class K, class T> static bool mapContains(std::map<K,T>&m , K key) {
        if (m.find(key) == m.end()) return false;
        return true;
    }

    template <class T> static bool mapContains(std::map<T, int>& amap, T key)
    {
        if (amap.find(key) == amap.end()) return false;
        return true;
    }

    static bool hasLoop(std::map<int, Node>& nodes);
    static bool hasLoopEasy(std::map<int, Node>& nodes, int start);
    static void topologicalSortUtil(int v, std::vector<bool>& visited, std::stack<int>& topstack, std::map<int, Node>& nodes);
    static std::stack<int> topSort(std::map<int, Node>& nodes);
    static bool isCircle(std::map<int, Node>& nodes, int from, int to);

};

#endif