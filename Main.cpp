//#pragma GCC optimize ("O3")
//#pragma GCC target ("avx")

#include <iostream>
#include <fstream>
#include "Coordinator.h"
#include "IOstuff.h"
#include <string>
#include "Utils.h"
#include "Mutate.h"

#include "Test.h"

/*using namespace std;
void * operator new(size_t size)
{
    cout << "New operator overloading " << endl;
    void * p = malloc(size);
    return p;
}

void operator delete(void * p)
{
    cout << "Delete operator overloading " << endl;
    free(p);
}*/

int main(int argc, char *argv[]) {

    Test tests;
   // tests.randomlyEvolveNeats(13, 4, 25, 50);
    tests.crashSaveLoad();

    return 0;
}

