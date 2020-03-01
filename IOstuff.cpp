#include "IOstuff.h"
#include <iostream>
#include <fstream>


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

void IOstuff::save(SaveData& saveData) {
    std::ofstream myfile;
    myfile.open(saveData.fileName);

    myfile << saveData.generation << "\n";
    neatInfoToStream(myfile, saveData.neat);
}

SaveData IOstuff::load(std::string filename) {
    std::ifstream myfile(filename);
    int generation;
    myfile >> generation;
    NEAT neat(myfile);
    return SaveData(neat,filename, generation);
}