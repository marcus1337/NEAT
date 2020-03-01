#include "NEAT.h"
#include <string>

#ifndef IOSTUFF_H
#define IOSTUFF_H

struct SaveData {
public:
    int generation;
    std::string fileName;
    NEAT neat;
    std::map<std::tuple<int, int, int>, int> takenNodeIDs;
    SaveData(NEAT& _neat, std::string _fileName, int _generation);
};

class IOstuff {
    static void neatInfoToStream(std::ofstream& stream, NEAT& neat);
    static void nodeIDsToStream(std::ofstream& stream, std::map<std::tuple<int, int, int>, int>& nodeIDs);
    static void loadNodeIDs(std::ifstream& stream, SaveData& savedata);
public:
    static void save(SaveData& saveData);
    static SaveData load(std::string filename);
};

#endif