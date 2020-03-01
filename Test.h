#include <iostream>
#include <fstream>
#include "Coordinator.h"
#include "IOstuff.h"
#include <string>
#include "Utils.h"
#include "Mutate.h"

#ifndef TEST_H
#define TEST_H

class Test {
public:

    void randomlyEvolveNeats(Coordinator& coordinator, int numIn, int numOut, int numAI, int steps);
    void crashSaveLoad();
    void printNeatInfo();

};

#endif