
#include "NEAT.h"
#include <string>

#ifndef IOSTUFF_H
#define IOSTUFF_H

class IOstuff {

public:

    static void save(NEAT& neat, int id);

    static NEAT load(std::string filename);

};

#endif