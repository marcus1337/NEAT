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

#include <future>

void twice(int m) {
    std::cout << std::string(std::to_string(m)+"\n");
    return;
}

void testFutures() {
    std::vector<std::future<void>> futures;
    for (int i = 0; i < 1000; ++i) {
        futures.push_back(std::async(std::launch::async | std::launch::deferred, twice, i));
    }

    for (auto &e : futures) {
       e.wait();
    }
}

int main(int argc, char *argv[]) {

   // testFutures();
    Test tests;
    tests.crashSaveLoad();

    return 0;
}

