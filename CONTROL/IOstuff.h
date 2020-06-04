#include "../NEAT.h"
#include <string>

#ifndef IOSTUFF_H
#define IOSTUFF_H
namespace NTE {
    struct SaveData {
    public:
        int generation;
        std::string fileName;
        NEAT neat;
        std::map<std::tuple<int, int, int>, int> takenNodeIDs;
        SaveData(NEAT& _neat, std::string _fileName, int _generation);
    };

    class IOstuff {
        std::string generationInfoFileName = "Generation_Info";
        std::string allSavesParentFileName = "NEAT_EVO";

        void makeFolder(std::string folderName);
        std::string getPath(std::string folderName);
        std::string getFolderName(int generation, std::string folderName);
        std::string getFilenameWithPath(std::string folderNameAndGeneration, int treeIndex);
        std::ofstream getFileOutStream(int treeIndex, int generation, std::string folderName);
        std::ifstream getFileInStream(int treeIndex, int generation, std::string folderName);
        std::ofstream getGenerationInfoOutStream(std::string folderName, int generation);
        std::ifstream getGenerationInfoInStream(std::string folderName, int generation);


        static void neatInfoToStream(std::ofstream& stream, NEAT& neat);
        static void nodeIDsToStream(std::ofstream& stream, std::map<std::tuple<int, int, int>, int>& nodeIDs);
        static void loadNodeIDs(std::ifstream& stream, SaveData& savedata);

    public:
        static void save(SaveData& saveData);
        static SaveData load(std::string filename);



        std::vector<NEAT> loadGeneration(int generation, std::string folderName = "NEATS");
        NEAT loadNEAT(int treeIndex, int generation, std::string folderName = "NEATS");
        void saveNEAT(NEAT& neat, int treeIndex, int generation, std::string folderName = "NEATS");
        void saveGeneration(std::vector<NEAT>& neats, int generation, std::string folderName = "NEATS");
    };
}
#endif