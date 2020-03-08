//#pragma GCC optimize ("O3")
//#pragma GCC target ("avx")
//#include "mimalloc-override.h"
//#include "mimalloc-new-delete.h"

#include <iostream>
#include <fstream>
#include "Coordinator.h"
#include "IOstuff.h"
#include <string>
#include "Utils.h"
#include "Mutate.h"

#include "Test.h"
#include <tuple>


int elegantPair(int x, int y) {
    if (x > y)
        return x * x + x + y;
    return y * y + x;
}


using namespace std;

int main(int argc, char *argv[]) {

    Test tests;
    tests.crashSaveLoad();

    return 0;
}

