
#include "NEAT.h"
#include "MutationRateControl.h"

#ifndef MUTATE_H
#define MUTATE_H
namespace NTE {
    class Mutate {

        static bool canAddGene(NEAT& neat, int from, int to);
        MutationRateControl mutationRateControl;
    public:

        static int maxNodes;

        static float newNodeRate, newLinkRate;
        static float randomizeLinkRate, mutateLinkRate;
        static float enableDisableLinkRate;
        static float extraMutationRate;

        static bool shouldMutate(float chance);

        static void recurrentLinkMutate(NEAT& neat);
        static void recurrentEnableDisableMutate(NEAT& neat);
        static void recurrentPointMutate(NEAT& neat);

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