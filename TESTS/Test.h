#include <iostream>
#include <fstream>
#include "../CONTROL/Coordinator.h"
#include "../CONTROL/IOstuff.h"
#include <string>
#include "../Utils.h"
#include "../Mutate.h"

#ifndef TEST_H
#define TEST_H

class Test {
public:

    void randomlyEvolveNeats(NTE::Coordinator& coordinator, int numIn, int numOut, int numAI, int steps);
    void crashSaveLoad();
    void evolveManyGenerations();
    void printNeatInfo();

};

#endif