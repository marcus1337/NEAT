#include "NEAT.h"
#include <string>

#ifndef IOSTUFF_H
#define IOSTUFF_H

struct SaveData {
    int generation;
    std::string fileName;
    NEAT neat;
    SaveData(NEAT& _neat, std::string _fileName, int _generation);
};

class IOstuff {
    static void neatInfoToStream(std::ofstream& stream, NEAT& neat);
public:
    static void save(SaveData& saveData);
    static SaveData load(std::string filename);
};

#endif