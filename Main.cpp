//#pragma GCC optimize ("O3")
//#pragma GCC target ("avx")

#include <iostream>
#include <list>
#include <fstream>
#include <tuple>
#include "NEAT.h"
#include "Innovator.h"

#define FOR(i,a,b) for(int i=a;i<b;i++)
#define rep(i,n) FOR(i,0,n)
#define ll int64_t

using namespace std;
void readStuff(istream& stream);

void init(int, int, int);

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    //istream& infile = cin;
    //ifstream infile("1.in");

    rep(i, 10) {
        init(8, 4, 10);
        Innovator::getInstance().reset();
    }

   

    std::cout << std::flush;



    return 0;
}

#include <cstdlib>
#include <ctime>
#include "Innovator.h"
#include "Mutate.h"


vector<NEAT> neats;

void init(int numIn, int numOut, int numAI) {
    srand(static_cast <unsigned> (time(0)));

    for(int i = 0 ; i <= numIn+numOut; i++)
        Innovator::getInstance().getAnyNodeNum();

    NEAT neat(numIn, numOut);
    float test[] = { 1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f };

    neat.calcOut(test);

   // for (int i = 0; i < numIn; i++) {
       // cout << "TEST: " << test[i] << endl;
       // for (Genome gen : neat.nodes[i].genomes) {
          //  cout << "FROM " << gen.getFrom() << " TO  " << gen.getTo() << endl;
          //  cout << "TEST: " << gen.weight << endl;
     //   }
   // }

    for (int i = 0; i < numOut; i++) {
         cout << "TEST: " << neat.outputs[i] << endl;
        // cout << "WOO "<< neat.outputs.data() << endl;
    }
    cout << endl;
    neat.reset();

  /*  std::clock_t start;
    double duration;
    start = std::clock();*/


    rep(i, 1) {
        rep(j, neat.gencopies.size()) {
            
            Genome& gen = neat.gencopies[j];
            gen.enabled = !gen.enabled;
            gen.childNodes++;
            gen.weight++;
            neat.updateGene(gen);
        }
    }
    /*duration = (std::clock() - start) / (double)((clock_t)1000);
    std::cout << "printf: " << duration << '\n';*/



    rep(i, 6000) {
         //   cout << i << " " << neat.nodes.size() << " g: " << neat.gencopies.size() << " busy " << neat.busyEdges.size()
          //      << "\n";
        //cout << i << " _ " << neat.nodes.size() << " " << neat.gencopies.size() << " " << neat.busyEdges.size() << " " << Innovator::getInstance().getMaxNodeNum() << " _ " << Innovator::getInstance().getMaxEdgeNum() << "\n";
        Mutate::allMutations(neat);
    }
    
    //neat.addGene(0, 1);
    //neat.addGene(1, 0);
    cout << "NODES " << neat.nodes.size() << " edges " << neat.gencopies.size() << endl;
    bool looped = neat.hasLoop();
    cout << "NEAT LOOP " << looped << endl;


 //   neat.calcOut(test);
    for (int i = 0; i < numOut; i++) {
        cout << "TESTB: " << neat.outputs[i] << endl;
    }


}


