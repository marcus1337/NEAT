#include "IOstuff.h"
#include <iostream>
#include <fstream>

#include <filesystem>
#include <iostream>
#include "../GenerationInfo.h"

#include "../Innovator.h"

using namespace NTE;
namespace fs = std::filesystem;

SaveData::SaveData(NEAT& _neat, std::string _fileName, int _generation) :
    neat(_neat), fileName(_fileName), generation(_generation) {}

void IOstuff::neatInfoToStream(std::ofstream& stream, NEAT& neat) {
    stream << neat.observedBehaviors[0] << " " << neat.observedBehaviors[1] << " " << neat.observedBehaviors[2]
        << "\n";
    stream << neat.numIn << " " << neat.numOut << "\n";
    stream << neat.fitness << "\n" << neat.gencopies.size() << "\n";
    for (size_t i = 0; i < neat.gencopies.size(); i++) {
        stream << neat.gencopies[i].getFrom() << " " << neat.gencopies[i].getTo() << " " << neat.gencopies[i].enabled
            << " " << neat.gencopies[i].weight << " " << neat.gencopies[i].childNodes << "\n";
    }
    /*for (size_t i = 0; i < neat.recurrentGeneCopies.size(); i++) {
        stream << neat.recurrentGeneCopies[i].getFrom() << " " << neat.recurrentGeneCopies[i].getTo() << " " << neat.recurrentGeneCopies[i].enabled
            << " " << neat.recurrentGeneCopies[i].weight << "\n";
    }*/
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

void IOstuff::loadNodeIDs(std::ifstream& stream, std::map<std::tuple<int, int, int>, int>& takenNodeIDs) {
    int numNodeIDs;
    stream >> numNodeIDs;
    for (int i = 0; i < numNodeIDs; i++) {
        int from, to, children, value;
        stream >> from >> to >> children >> value;
        std::tuple<int, int, int> betweenEdges = std::make_tuple(from, to, children);
        takenNodeIDs[betweenEdges] = value;
    }
}

SaveData IOstuff::load(std::string filename) {
    std::ifstream myfile(filename);
    int generation;
    myfile >> generation;
    NEAT neat(myfile);
    SaveData saveData(neat, filename, generation);
    loadNodeIDs(myfile, saveData.takenNodeIDs);
    return saveData;
}





std::vector<NEAT> IOstuff::loadGeneration(int generation, std::string folderName) {
    std::vector<NEAT> result;
    GenerationInfo generationInfo;
    generationInfo.generation = generation;
    std::ifstream infoStream = getGenerationInfoInStream(folderName, generation, generationInfoFileName);
    generationInfo.loadData(infoStream);

    std::map<std::tuple<int, int, int>, int> takenNodeIDs;
    std::ifstream innovNumsStream = getGenerationInfoInStream(folderName, generation, innovationNumbersFileName);
    loadNodeIDs(innovNumsStream, takenNodeIDs);
    Innovator::getInstance().setTakenNodeIDs(takenNodeIDs);

    for (int i = 0; i < generationInfo.numNEATs; i++) {
        NEAT neat = loadNEAT(i + 1, generation, folderName);
        result.push_back(neat);
    }
    return result;
}

void IOstuff::saveGeneration(std::vector<NEAT>& neats, int generation, std::string folderName) {
    std::ofstream infoStream = getGenerationInfoOutStream(folderName, generation, generationInfoFileName);
    std::ofstream infoInnovNumsStream = getGenerationInfoOutStream(folderName, generation, innovationNumbersFileName);
    
    std::map<std::tuple<int, int, int>, int> takenNodeIDs = Innovator::getInstance().getAllNodeIDs();
    nodeIDsToStream(infoInnovNumsStream, takenNodeIDs);

    GenerationInfo generationInfo;
    generationInfo.generation = generation;
    generationInfo.saveData(infoStream, neats);
    for (size_t i = 0; i < neats.size(); i++)
        saveNEAT(neats[i], (int)(i + 1), generation, folderName);
}

NEAT IOstuff::loadNEAT(int treeIndex, int generation, std::string folderName) {
    std::ifstream stream = getFileInStream(treeIndex, generation, folderName);
    return NEAT(stream);
}

void IOstuff::saveNEAT(NEAT& neat, int treeIndex, int generation, std::string folderName) {
    std::ofstream stream = getFileOutStream(treeIndex, generation, folderName);
    neatInfoToStream(stream, neat);
}

void IOstuff::saveNEAT(NEAT& neat, std::string folderName, std::string fileName) {
    makeFolder(folderName);
    std::string filePath = std::filesystem::current_path().string();
    filePath += "\\" + folderName + "\\" + fileName;
    std::ofstream stream(filePath);
    neatInfoToStream(stream, neat);
}

NEAT IOstuff::loadNEAT(std::string folderName, std::string fileName) {
    std::string filePath = std::filesystem::current_path().string();
    filePath += "\\" + folderName + "\\" + fileName;
    std::ifstream stream(filePath);
    return NEAT(stream);
}

void IOstuff::makeFolder(std::string folderName) {
    std::string filePath = getPath(folderName);
    if (!fs::exists(filePath))
        fs::create_directories(filePath);
    if (!fs::exists(filePath))
        fs::create_directory(filePath);
}
std::string IOstuff::getPath(std::string fileName) {
    std::string filePath = std::filesystem::current_path().string();
    filePath += "\\" + allSavesParentFileName + "\\" + fileName;
    return filePath;
}
std::string IOstuff::getFolderName(int generation, std::string folderName) {
    return std::string(folderName + "_" + std::to_string(generation));
}
std::string IOstuff::getFilenameWithPath(std::string folderNameAndGeneration, int treeIndex) {
    return getPath(std::string(folderNameAndGeneration + "//TREE_" + std::to_string(treeIndex) + ".txt"));
}
std::ofstream IOstuff::getFileOutStream(int treeIndex, int generation, std::string folderName) {
    std::string folderNameAndGeneration = getFolderName(generation, folderName);
    makeFolder(folderNameAndGeneration);
    return std::ofstream(getFilenameWithPath(folderNameAndGeneration, treeIndex));
}
std::ifstream IOstuff::getFileInStream(int treeIndex, int generation, std::string folderName) {
    std::string folderNameAndGeneration = getFolderName(generation, folderName);
    makeFolder(folderNameAndGeneration);
    return std::ifstream(getFilenameWithPath(folderNameAndGeneration, treeIndex));
}

std::ofstream IOstuff::getGenerationInfoOutStream(std::string folderName, int generation, std::string info_fileName) {
    std::string folderNameAndGeneration = getFolderName(generation, folderName);
    makeFolder(folderNameAndGeneration);
    return std::ofstream(getPath(std::string(folderNameAndGeneration + "//" + info_fileName + ".txt")));
}

std::ifstream IOstuff::getGenerationInfoInStream(std::string folderName, int generation, std::string info_fileName) {
    std::string folderNameAndGeneration = getFolderName(generation, folderName);
    makeFolder(folderNameAndGeneration);
    return std::ifstream(getPath(std::string(folderNameAndGeneration + "//" + info_fileName + ".txt")));
}


std::map<std::tuple<int, int, int>, NEAT> IOstuff::loadElites(std::string folderName) {
    std::map<std::tuple<int, int, int>, NEAT> result;

    auto stream = std::ifstream(getPath(std::string(folderName + "//" + "elites_info" + ".txt")));
    size_t numNeats;
    stream >> numNeats;

    for (size_t i = 0; i < numNeats; i++) {
        auto stream = std::ifstream(getFilenameWithPath(folderName, (int)i));
        NEAT neat(stream);
        result[std::make_tuple(neat.observedBehaviors[0], neat.observedBehaviors[1],
            neat.observedBehaviors[2])] = neat;
    }

    return result;
}
void IOstuff::saveElites(std::map<std::tuple<int, int, int>, NEAT>& elites, std::string folderName) {
    makeFolder(folderName);
    auto stream = std::ofstream(getPath(std::string(folderName + "//" + "elites_info" + ".txt")));
    stream << elites.size();

    int treeIndex = 0;
    for (auto& neat : elites)
    {
        auto stream = std::ofstream(getFilenameWithPath(folderName, treeIndex));
        neatInfoToStream(stream, neat.second);
        treeIndex++;
    }

}