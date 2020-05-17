#include "Speciator.h"

#ifndef STANDARDSTRATEGY_H
#define STANDARDSTRATEGY_H
namespace NTE {
    class StandardStrategy : public Speciator {
    public:
        StandardStrategy() : Speciator() {}

        void fitnessSharing(std::vector<NEAT>& neats);
        void newGeneration(std::vector<std::future<void>>& futures);
        void cullSpecies();

    };
}
#endif