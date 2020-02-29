#pragma once

#include "NEAT.h"

class Mutate {

public:

    static float mutationrate, mutationrateNewNode;
    //static constexpr float mutationrate = 0.99f;
    //static constexpr float pointrate = 0.01f;

    static bool shouldMutate(float chance);

    static void linkMutate(NEAT& neat);
    static void nodeMutate(NEAT& neat);
    static void enableDisableMutate(NEAT& neat);
    static void pointMutate(NEAT& neat);

    static void allMutations(NEAT& neat);

};