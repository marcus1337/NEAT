
#include <iostream>
#include <fstream>
#include "../CONTROL/Coordinator.h"
#include "../CONTROL/IOstuff.h"
#include <string>
#include "../Utils.h"
#include "../Mutate.h"
#include "Test.h"
#include <tuple>
#include <future>

using namespace std;
using namespace NTE;


int main(int argc, char *argv[]) {

    Test tests;
    tests.crashSaveLoad();
    //tests.testElites();

    return 0;
}

