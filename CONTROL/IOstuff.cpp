#include "IOstuff.h"
#include <iostream>
#include <fstream>
using namespace NTE;

SaveData::SaveData(NEAT& _neat, std::string _fileName, int _generation) :
    neat(_neat), fileName(_fileName), generation(_generation) {}

void IOstuff::neatInfoToStream(std::ofstream& stream, NEAT& neat) {
    stream << neat.numIn << " " << neat.numOut << "\n";
    stream << neat.fitness << "\n" << neat.gencopies.size() << "\n";
    for (int i = 0; i < neat.gencopies.size(); i++) {
        stream << neat.gencopies[i].getFrom() << " " << neat.gencopies[i].getTo() << " " << neat.gencopies[i].enabled
            << " " << neat.gencopies[i].weight << " " << neat.gencopies[i].childNodes << "\n";
    }
}

void IOstuff::nodeIDsToStream(std::ofstream& stream, std::map<std::tuple<int, int, int>, int>& nodeIDs) {
    stream << nodeIDs.size() << "\n";
    for (const auto &[key, value] : nodeIDs) {
        stream << std::get<0>(key) << " " << std::get<1>(key) << " " << std::get<2>(key)
            << " " << value << "\n";
    }
}

void IOstuff::save(SaveData& saveData) {
    std::ofstream myfile;
    myfile.open(saveData.fileName);
    myfile << saveData.generation << "\n";
    neatInfoToStream(myfile, saveData.neat);
    nodeIDsToStream(myfile, saveData.takenNodeIDs);
}

void IOstuff::loadNodeIDs(std::ifstream& stream, SaveData& saveData) {
    int numNodeIDs;
    stream >> numNodeIDs;
    for (int i = 0; i < numNodeIDs; i++) {
        int from, to, children, value;
        stream >> from >> to >> children >> value;
        std::tuple<int, int, int> betweenEdges = std::make_tuple(from, to, children);
        saveData.takenNodeIDs[betweenEdges] = value;
    }
}

SaveData IOstuff::load(std::string filename) {
    std::ifstream myfile(filename);
    int generation;
    myfile >> generation;
    NEAT neat(myfile);
    SaveData saveData(neat, filename, generation);
    loadNodeIDs(myfile, saveData);
    return saveData;
}