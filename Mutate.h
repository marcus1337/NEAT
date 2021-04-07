
#include "NEAT.h"
#include "MutationRateControl.h"

#ifndef MUTATE_H
#define MUTATE_H
namespace NTE {
    class Mutate {

        static bool canAddGene(NEAT& neat, int from, int to);
        MutationRateControl mutationRateControl;
    public:

        static int maxHiddenNodes;

        static float newNodeRate, newLinkRate;
        static float randomizeLinkRate, mutateLinkRate;
        static float enableDisableLinkRate;
        static float extraMutationRateScalar;

        static bool isExtraMutationActive;

        static bool shouldMutate(float chance);

        static void linkMutate(NEAT& neat);
        static void nodeMutate(NEAT& neat);
        static void enableDisableMutate(NEAT& neat);
        static void pointMutate(NEAT& neat);
        static void allMutations(NEAT& neat);

        static int getUnusedNodeID(const std::map<int, Node>& nodes, int from, int to);

        void modifyMutationRate(std::vector<NEAT>& neat);

    };
}
#endif