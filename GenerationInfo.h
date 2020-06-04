#include <vector>
#include <fstream>
#include "NEAT.h"

#ifndef GENERATIONINFO_H
#define GENERATIONINFO_H
namespace NTE {
    class GenerationInfo {
        void storeData(std::vector<NEAT>& trees);
    public:
        std::vector<int> fitnesses;
        float averageFitness;
        int maxFitness;
        int minFitness;
        int generation;
        int numNEATs;

        GenerationInfo();
        void saveData(std::ofstream& stream, std::vector<NEAT>& trees);
        void loadData(std::ifstream& stream);
    };
}
#endif