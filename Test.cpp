#include "Test.h"

#include "Innovator.h"

#define FOR(i,a,b) for(int i=a;i<b;i++)
#define rep(i,n) FOR(i,0,n)
#define ll int64_t
using namespace std;

#include <chrono>

void Test::crashSaveLoad() {
    Coordinator coordinator;
    int numIn = 13, numOut = 4, numAI = 100;
    coordinator.init(numIn, numOut, numAI);
    std::string fileName = "NEAT_SAVE77";
    //coordinator.save(0, fileName);
    std::vector<float> inputs = {1,2,0,-4,5,3,-4,4,5,2,3,5,4,0,3.4f,3.6f,4.2f};
    for (int i = 0; i < 10*3; i++) {

        auto t1 = std::chrono::high_resolution_clock::now();
        //coordinator.load(fileName, numAI);
        randomlyEvolveNeats(coordinator, numIn, numOut, numAI, 10);
        //coordinator.save(0, fileName);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << duration << "\n";

        rep(i, (*coordinator.neatBuffer.neats).size()) {
            (*coordinator.neatBuffer.neats)[i].calculateOutput(inputs.data());
        }
    }
}

void Test::evolveManyGenerations() {
    Coordinator coordinator;
    int numIn = 13, numOut = 4, numAI = 50;
    coordinator.init(numIn, numOut, numAI);
    std::vector<float> inputs = { 1,2,0,-4,5,3,-4,4,5,2,3,5,4,0,3.4f,3.6f,4.2f };
    for (int i = 0; i < 10; i++) {

        randomlyEvolveNeats(coordinator, numIn, numOut, numAI, 50);

        rep(i, (*coordinator.neatBuffer.neats).size()) {
            (*coordinator.neatBuffer.neats)[i].calculateOutput(inputs.data());
        }
    }
}


void Test::printNeatInfo() {
    NEAT aa(13, 4);
    for (int i = 0; i < aa.gencopies.size(); i++) {
        Genome& g = aa.gencopies[i];
        cout << "A " << g.getFrom() << " " << g.getTo()
            << " TYP " << aa.nodes[g.getFrom()].getType()
            << " _ " << aa.nodes[g.getTo()].getType() << "\n";
    }
    for (const auto& n : aa.nodes) {
        cout << "TEST " << n.second.getType() << "\n";
    }
}

void Test::randomlyEvolveNeats(Coordinator& coordinator, int numIn, int numOut, int numAI, int steps) {
    rep(i, (*coordinator.neatBuffer.neats).size()) {
        (*coordinator.neatBuffer.neats)[i].fitness = Utils::randi(1, 1000);
    }
    rep(i, steps) {
        coordinator.evolve(); 
        rep(i, (*coordinator.neatBuffer.neats).size()) {
            (*coordinator.neatBuffer.neats)[i].fitness += Utils::randi(-2, 2);
        }
       // cout << "TEST " << (*coordinator.neatBuffer.neats)[0].nodes.size() << " e " << (*coordinator.neatBuffer.neats)[0].getNumGenes() << "\n";
       // cout << "inno " << Innovator::getInstance().geneNum << " , " << Innovator::getInstance().nodeNum << "\n";
    }
}