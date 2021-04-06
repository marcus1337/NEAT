#include "Test.h"

#include "../Innovator.h"

#define FOR(i,a,b) for(int i=a;i<b;i++)
#define rep(i,n) FOR(i,0,n)
#define ll int64_t
using namespace std;

#include <chrono>
using namespace NTE;


void Test::loadElites() {
    Coordinator coordinator;
    int numIn = 13, numOut = 4, numAI = 5;
    coordinator.init(numIn, numOut, numAI);
    std::string fileName = "NEAT_SAVE1337";
    coordinator.loadElites(fileName);

    std::vector<float> inputs = { 1,2,0,-4,5,3,-4,4,5,2,3,5,4,0,3.4f,3.6f,4.2f };
    int eliteNum = 5;
    coordinator.calcEliteInput(eliteNum,inputs);
    auto tmp = coordinator.getEliteOutput(eliteNum);

    cout << "TEST " << tmp[0] << endl;
}

void Test::saveLoadElites() {
    Coordinator coordinator;
    int numIn = 13, numOut = 4, numAI = 5;
    coordinator.init(numIn, numOut, numAI);
    std::string fileName = "NEAT_SAVE1337";

    for(int i = 0 ; i < numAI; i++)
        coordinator.setFitness(i, 1*i);

    for (int i = 0; i < numAI; i++)
        coordinator.setBehavior(i, { i,i,i });

    coordinator.evolve();

    coordinator.saveElites(fileName);

}

void Test::crashSaveLoad() {
    Coordinator coordinator;
    int numIn = 13, numOut = 4, numAI = 5;
    coordinator.init(numIn, numOut, numAI);
    std::string fileName = "NEAT_SAVE77";
    //coordinator.save(0, fileName);
    std::vector<float> inputs = {1,2,0,-4,5,3,-4,4,5,2,3,5,4,0,3.4f,3.6f,4.2f};
    for (int i = 0; i < 100*3; i++) {

        auto t1 = std::chrono::high_resolution_clock::now();
        //coordinator.load(fileName, numAI);
        randomlyEvolveNeats(coordinator, numIn, numOut, numAI, 10);
        //coordinator.save(0, fileName);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << duration << "\n";

        rep(i, (*coordinator.neatBuffer.neats).size()) {
            (*coordinator.neatBuffer.neats)[i].calculateOutput(inputs);
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
            (*coordinator.neatBuffer.neats)[i].calculateOutput(inputs);
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

void Test::randomlyEvolveNeats(NTE::Coordinator& coordinator, int numIn, int numOut, int numAI, int steps) {
    rep(i, steps) {
        rep(i, (*coordinator.neatBuffer.neats).size()) {
            (*coordinator.neatBuffer.neats)[i].fitness = Utils::randi(1, 1000);
        }

        coordinator.evolve(); 
        rep(i, (*coordinator.neatBuffer.neats).size()) {
            (*coordinator.neatBuffer.neats)[i].fitness += Utils::randi(-2, 2);
        }
        //cout << "TEST " << (*coordinator.neatBuffer.neats)[0].nodes.size() << " e " << (*coordinator.neatBuffer.neats)[0].getNumGenes() <<
        //    "  nodes: " << Innovator::getInstance().nodeNum << "\n";
    }
}

void Test::testElites() {
    Coordinator coordinator;
    int numIn = 13, numOut = 4, numAI = 50;
    coordinator.init(numIn, numOut, numAI);
    std::vector<float> inputs = { 1,2,0,-4,5,3,-4,4,5,2,3,5,4,0,3.4f,3.6f,4.2f };
    
    for (int i = 0; i < 50; i++) {
        coordinator.randomizePopulation(5, 5);
        coordinator.setFitness(0, 500);
        coordinator.neatBuffer.neats->at(0).observedBehaviors = { 5,30,10 };
        coordinator.storeElites();
        cout << "HEJ: " << i << endl;
    }
}

std::vector<float> Test::getRandomFloatArray(int arrLen) {
    std::vector<float> result(arrLen);
    for (int i = 0; i < arrLen; i++) {
        result[i] = (float)Utils::randi(0, 1);
    }
    return result;
}

void Test::randomEvolutionAndRandomizeFromElites() {
    int numIn = 200, numOut = 5, numAI = 200;

    for (int i = 0; i < 3; i++) {
        Coordinator coordinator;
        coordinator.init(numIn, numOut, numAI);
        coordinator.setMaxHiddenNodes(170);
        coordinator.setSurpriseEffect(0.07);
        std::cout << "STARTING TEST...\n";
        testRandomizationOfElites(numAI, numIn, coordinator);
    }


}

void Test::testRandomizationOfElites(int numAI, int numIn, NTE::Coordinator &coordinator)
{
    for (int n = 0; n < 50; n++) {
        for (int j = 0; j < numAI; j++) {
            std::vector<float> inputs = getRandomFloatArray(numIn);
            coordinator.calcInput(j, inputs);
            coordinator.getOutput(j);
            coordinator.setFitness(j, Utils::randi(0, 1000));
            std::vector<int> behaves = { Utils::randi(0, 100), Utils::randi(0, 100), Utils::randi(0, 100) };
            coordinator.setBehavior(j, behaves);
        }
        std::cout << "GENERATION: " << n << std::endl;
        if (n % 3 == 0) {
            coordinator.storeElites();
        }
        if (n % 5 == 0) {
            coordinator.randomizePopulationFromElites();
        }
    }
}


void Test::testSavedEliteRemainTheSameAfterLoad() {
    int numIn = 2, numOut = 2, numAI = 200;
    Coordinator coordinator;
    coordinator.init(numIn, numOut, numAI);

    coordinator.setFitness(0, 100);
    coordinator.setBehavior(0, { 1,1,1 });
    coordinator.storeElites();


    std::string filename = "blubb1.file";
    std::string fileDir = "blubbdirectory1";
    NEAT oldNEAT = (*coordinator.neatBuffer.neats)[0];
    coordinator.saveBestElite(filename, fileDir);
    coordinator.loadBestElite(filename, fileDir);
   /* NEAT newNEAT;
    for (const auto& elites : coordinator.evolver.mapElites.eliteNEATs) {
        if (elites.second.fitness == 100) {
            std::cout << "found something...\n";
            newNEAT = elites.second;
            break;
        }
    }*/

    NEAT newNEAT = (*coordinator.neatBuffer.neats)[0];
    cout << "FITNESS: " << newNEAT.fitness << endl;

    //int,node
    std::vector<std::pair<int, Node>> nodes1;
    std::vector<std::pair<int, Node>> nodes2;

    for (const auto& nod : oldNEAT.nodes) {
        nodes1.push_back(nod);
    }
    for (const auto& nod : newNEAT.nodes) {
        nodes2.push_back(nod);
    }

    std::cout << "SIZE 1: " << newNEAT.gencopies.size() << std::endl;
    std::cout << "SIZE 2: " << oldNEAT.gencopies.size() << std::endl;

    for (int i = 0; i < newNEAT.gencopies.size(); i++) {
        std::cout << "{" << newNEAT.gencopies[i].getID() << "," << newNEAT.gencopies[i].getFrom()
            << "," << newNEAT.gencopies[i].getTo() << "}";
        std::cout << "{" << oldNEAT.gencopies[i].getID() << "," << oldNEAT.gencopies[i].getFrom()
            << "," << oldNEAT.gencopies[i].getTo() << "}";
        std::cout << " -_- " << newNEAT.gencopies[i].weight << " -_- " << oldNEAT.gencopies[i].weight;


        std::cout << std::endl;
    }

    for (int i = 0; i < nodes1.size(); i++) {
        if (nodes1[i].first != nodes2[i].first || nodes1[i].second.getID() != nodes2[i].second.getID() ||
            nodes1[i].second.genomes.size() != nodes2[i].second.genomes.size()) {
            std::cout << "ERROR....... " << i << "\n\n";
            std::cout << nodes1[i].first << " _ " << nodes2[i].first << std::endl;
            std::cout << nodes1[i].second.getID() << " _ " << nodes2[i].second.getID() << std::endl;
            std::cout << nodes1[i].second.genomes.size() << " _ " << nodes2[i].second.genomes.size() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

}


void Test::printNEATInfo(int ID) {
    Coordinator coordinator;
    std::string filename = "ELITE_" + std::to_string(ID);
    coordinator.loadBestElite(filename);
    auto usedIDs = (*coordinator.neatBuffer.neats)[0].getUsedNodeIDs();
    std::cout << "NUM USED HIDDEN NODES: " << usedIDs.size() << std::endl;
    std::cout << "TOTAL NUM HIDDEN NODES: " << (*coordinator.neatBuffer.neats)[0].getNumGenes() - (*coordinator.neatBuffer.neats)[0].numIn - (*coordinator.neatBuffer.neats)[0].numOut << std::endl;
    auto usedGenes = (*coordinator.neatBuffer.neats)[0].getUsedGenomes();
    std::cout << "NUM USED GENES: " << usedGenes.size() << std::endl;
    std::cout << "TOTAL NUM GENES: " << (*coordinator.neatBuffer.neats)[0].gencopies.size() << std::endl;
    
}