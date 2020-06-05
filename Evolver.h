#include "Speciator.h"
#include "NEAT.h"
#include "Breeder.h"
#include "Selector.h"
#include "Surprise.h"

#ifndef EVOLVER_H
#define EVOLVER_H

namespace NTE {
    class Evolver {

    public:

        Surprise surprise;
        Selector selector;
        Breeder breeder;
        Speciator speciator;
        void makeNewGeneration(std::vector<NEAT>& neats, std::vector<NEAT>& oldNeats);

    };
}


#endif // !EVOLVER_H