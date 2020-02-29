//#pragma GCC optimize ("O3")
//#pragma GCC target ("avx")

#include <iostream>
#include <fstream>
#include "Coordinator.h"
#include "IOstuff.h"
#include <string>
#include "Utils.h"
#include "Mutate.h"

#define FOR(i,a,b) for(int i=a;i<b;i++)
#define rep(i,n) FOR(i,0,n)
#define ll int64_t

using namespace std;

void init(int, int, int);

Coordinator coordinator;

void tester();

const int testNum = 5;

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    init(13, 4, testNum);
    //tester();

    std::cout << std::flush;

    return 0;
}

void tester() {
    NEAT aa(13, 4);

    for (int i = 0; i < aa.gencopies.size(); i++) {
        Genome& g = aa.gencopies[i];
        cout << "A " << g.getFrom() << " " << g.getTo() << " TYP " << aa.nodes[g.getFrom()].getType() << " _ "
            << aa.nodes[g.getTo()].getType() << endl;
    }

    for (const auto& n : aa.nodes) {
        cout << "TEST " << n.second.getType() << "\n";
    }
}



float test[] = { 1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f, 1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f };

void init(int numIn, int numOut, int numAI) {
    coordinator.init(numIn, numOut, numAI);

    rep(i, coordinator.neats.size()) {
        coordinator.neats[i].fitness = Utils::randi(1,1000);
    }

    rep(i, 5000) {

        coordinator.evolve();
        rep(i, coordinator.neats.size()) {
            coordinator.neats[i].fitness += Utils::randi(-2, 2);
        }

        cout << "TEST " << coordinator.neats[0].nodes.size() << " e " << coordinator.neats[0].gencopies.size() << endl;
    }

}
