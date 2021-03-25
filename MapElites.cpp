
#include "MapElites.h"

#include <functional>
#include <iterator> 
#include "Utils.h"

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
    return eliteNEATs[_key].fitness < neat.fitness
        || (eliteNEATs[_key].fitness == neat.fitness);
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

void MapElites::randomElitism(std::vector<NEAT>& NEATs) {
    if (eliteNEATs.empty())
        return;
    std::vector<NEAT*> allElites = getElitesVector();
    std::cout << "NUM ELITES: " << allElites.size() << std::endl;
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