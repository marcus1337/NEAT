//#pragma GCC optimize ("O3")
//#pragma GCC target ("avx")
#include "mimalloc-override.h"
#include "mimalloc-new-delete.h"

#include <iostream>
#include <fstream>
#include "Coordinator.h"
#include "IOstuff.h"
#include <string>
#include "Utils.h"
#include "Mutate.h"

#include "Test.h"


int main(int argc, char *argv[]) {

    Test tests;
   // tests.randomlyEvolveNeats(13, 4, 25, 50);
    tests.crashSaveLoad();

    return 0;
}

