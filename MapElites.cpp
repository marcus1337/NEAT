
#include "MapElites.h"

#include <functional>
#include <iterator> 
#include "Utils.h"
#include <algorithm> 
#include <iostream>

using namespace NTE;

std::map<std::tuple<int, int, int>, NEAT> MapElites::eliteNEATs;

bool MapElites::isOccupied(std::tuple<int, int, int> key) {
    std::map<std::tuple<int, int, int>, NEAT>::iterator it = eliteNEATs.find(key);
    if (it != eliteNEATs.end())
        return true;
    return false;
}

bool MapElites::isOccupied(int a, int b, int c) {
    std::map<std::tuple<int, int, int>, NEAT>::iterator it = eliteNEATs.find(std::make_tuple(a, b, c));
    if (it != eliteNEATs.end())
        return true;
    return false;
}

bool MapElites::isEliteBetter(int a, int b, int c, NEAT& NEAT) {
    return isOccupied(a, b, c) && eliteNEATs[getKey(NEAT)].fitness > NEAT.fitness;
}

void MapElites::storeNEAT(int a, int b, int c, NEAT& newNEAT) {
    eliteNEATs[std::make_tuple(a, b, c)] = newNEAT;
}

NEAT MapElites::getNEAT(int a, int b, int c) {
    return eliteNEATs[std::make_tuple(a, b, c)];
}

void MapElites::mapOrStoreElite(NEAT& NEAT) {
    auto _key = getKey(NEAT);
    if (!isOccupied(_key) || isNewNEATBetter(_key, NEAT))
        eliteNEATs[_key] = NEAT;
    else
        NEAT = eliteNEATs[_key];
}

bool MapElites::isNewNEATBetter(std::tuple<int, int, int> _key, NEAT& neat) {
    return eliteNEATs[_key].fitness < neat.fitness;
}

void MapElites::mapOrStoreElites(std::vector<NEAT>& NEATs) {
    for (size_t i = 0; i < NEATs.size(); i++)
        mapOrStoreElite(NEATs[i]);
}

void MapElites::storeElite(NEAT& neat) {
    auto _key = getKey(neat);
    if (!isOccupied(_key) || isNewNEATBetter(_key, neat)) {
        eliteNEATs[_key] = neat;
    }
}

void MapElites::storeElites(std::vector<NEAT>& NEATs) {
    for (size_t i = 0; i < NEATs.size(); i++)
        storeElite(NEATs[i]);
}

std::vector<NEAT*> MapElites::getElitesVector() {
    std::vector<NEAT*> allElites;
    std::transform(eliteNEATs.begin(), eliteNEATs.end(), std::back_inserter(allElites),
        [](auto &kv) { return &kv.second; });
    return allElites;
}

std::vector<NEAT> MapElites::getElitesVectorCopy() {
    std::vector<NEAT> allElites;
    allElites.reserve(eliteNEATs.size());
    for (const auto& elite : eliteNEATs)
        allElites.push_back(elite.second);
    return allElites;
}

void MapElites::randomElitism(std::vector<NEAT>& NEATs) {
    if (eliteNEATs.empty())
        return;
    std::vector<NEAT*> allElites = getElitesVector();
    for (size_t i = 0; i < NEATs.size(); i++) {
        int randomEliteIndex = Utils::randi(0, allElites.size() - 1);
        NEATs[i] = *allElites[randomEliteIndex];
    }
}

std::tuple<int, int, int> MapElites::getKey(NEAT& NEAT) {
    int a = NEAT.observedBehaviors[0];
    int b = NEAT.observedBehaviors[1];
    int c = NEAT.observedBehaviors[2];
    return std::make_tuple(a, b, c);
}


int MapElites::getNumElites() {
    return eliteNEATs.size();
}

int MapElites::getNumElitesOfUniqueDimensionValue(int dimension) {
    if (dimension > 2 || dimension < 0)
        return -1;
    std::set<int> foundKeys;
    for (const auto& elite : eliteNEATs) {
        if (dimension == 0)
            foundKeys.insert(std::get<0>(elite.first));
        if (dimension == 1)
            foundKeys.insert(std::get<1>(elite.first));
        if(dimension == 2)
            foundKeys.insert(std::get<2>(elite.first));
    }
    return foundKeys.size();
}

int MapElites::getNumElitesOfDimensionWithValue(int dimension, int value) {
    if (dimension > 2 || dimension < 0)
        return -1;
    return std::count_if(std::begin(eliteNEATs),
        std::end(eliteNEATs),
        [dimension, value](std::pair<std::tuple<int, int, int>, NEAT> const &p) {
        if(dimension == 0)
            return std::get<0>(p.first) == value;
        if (dimension == 1)
            return std::get<1>(p.first) == value;
        return std::get<2>(p.first) == value;
    });
}
int MapElites::getNumElitesOfDimensionWithinThreshold(int dimension, int low, int high) {
    if (dimension > 2 || dimension < 0)
        return -1;
    return std::count_if(std::begin(eliteNEATs),
        std::end(eliteNEATs),
        [dimension, low, high](std::pair<std::tuple<int, int, int>, NEAT> const &p) {
        if (dimension == 0)
            return std::get<0>(p.first) >= low && std::get<0>(p.first) <= high;
        if (dimension == 1)
            return std::get<1>(p.first) >= low && std::get<1>(p.first) <= high;
        return std::get<2>(p.first) >= low && std::get<2>(p.first) <= high;
    });
}