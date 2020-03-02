#include "NEAT.h"

#ifndef NEATDOUBLEBUFFER_H
#define NEATDOUBLEBUFFER_H

class NEATDoubleBuffer {
    std::vector<NEAT> neatBuff1;
    std::vector<NEAT> neatBuff2;
public:
    std::vector<NEAT>* neats;
    std::vector<NEAT>* oldNeats;

    NEATDoubleBuffer();
    void prepareNewGeneration();
    void swapBuffers();
};

#endif