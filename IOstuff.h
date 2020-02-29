#pragma once
#include "NEAT.h"
#include <string>

class IOstuff {

public:

    static void save(NEAT& neat, int id);

    static NEAT load(std::string filename);

};