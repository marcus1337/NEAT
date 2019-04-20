#pragma once
#include <cstdlib>
#include <ctime>
#include "Innovator.h"
#include "Mutate.h"
#include "NEAT.h"
#include <list>
#include <tuple>
#include <vector>
#include "Speciator.h"

class Coordinator {

public:
    std::vector<NEAT> neats;
    int _numAI, _numOut, _numIn;

    Speciator speciator;

    void init(int numIn, int numOut, int numAI) {
        srand(static_cast <unsigned> (time(0)));
        _numAI = numAI;
        _numOut = numOut;
        _numIn = numIn;
        Innovator::getInstance().reset();
        for (int i = 0; i <= numIn + numOut; i++)
            Innovator::getInstance().getAnyNodeNum();

        for (int i = 0; i < numAI; i++) {
            neats.push_back(NEAT(numIn, numOut));
        }
        speciator.init(numIn, numOut, numAI);
    }

    void evolve() {

       // std::cout << "BEFORE " << neats.size() << std::endl;
        speciator.speciate(neats);
      //  std::cout << "AFTER " << neats.size() << std::endl;
    }


    void calcInput(int index, float* inputs) {
        neats[index].topSortCalc(inputs);
    }

    float* getOutput(int index) {
        return neats[index].outputs.data();
    }

    void setFitness(int index, int fitness) {
        neats[index].fitness = fitness;
    }


};