#include "Test.h"


#define FOR(i,a,b) for(int i=a;i<b;i++)
#define rep(i,n) FOR(i,0,n)
#define ll int64_t
using namespace std;



void Test::crashSaveLoad() {
    Coordinator coordinator;
    coordinator.init(13, 4, 100);
    std::string fileName = "NEAT_SAVE77";
    coordinator.save(0, fileName);
    std::vector<float> inputs = {1,2,0,-4,5,3,-4,4,5,2,3,5,4,0,3.4f,3.6f,4.2f};
    for (int i = 0; i < 10; i++) {
        coordinator.load(fileName, 20);
        randomlyEvolveNeats(coordinator, 13, 4, 20, 100);
        coordinator.save(0, fileName);

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
        cout << "TEST " << (*coordinator.neatBuffer.neats)[0].nodes.size() << " e " << (*coordinator.neatBuffer.neats)[0].gencopies.size() << "\n";
    }
}