
#include <vector>
#include "NEAT.h"

#include <map>
#include <algorithm>
#include <vector>
#include <tuple>

#include <string>
#include <unordered_map>

#ifndef MAPELITES_H
#define MAPELITES_H

namespace NTE {
    class MapElites {

    public:
       
        int getNumElites();
        int getNumElitesOfUniqueDimensionValue(int dimension);
        int getNumElitesOfDimensionWithValue(int dimension, int value);
        int getNumElitesOfDimensionWithinThreshold(int dimension, int low, int high);

        static std::map<std::tuple<int, int, int>, NEAT> eliteNEATs;

        bool isOccupied(std::tuple<int, int, int> key);
        bool isOccupied(int a, int b, int c);
        bool isEliteBetter(int a, int b, int c, NEAT& newNEAT);
        void storeNEAT(int a, int b, int c, NEAT& newNEAT);
        NEAT getNEAT(int a, int b, int c);

        void mapOrStoreElite(NEAT& oldNEAT);
        void mapOrStoreElites(std::vector<NEAT>& NEATs);
        bool isNewNEATBetter(std::tuple<int, int, int> key, NEAT& newNEAT);

        std::vector<NEAT*> getElitesVector();
        std::vector<NEAT> getElitesVectorCopy();

        void storeElite(NEAT& NEAT);
        void storeElites(std::vector<NEAT>& NEATs);
        std::tuple<int, int, int> getKey(NEAT& NEAT);
        void randomElitism(std::vector<NEAT>& NEATs);
    };
}

#endif