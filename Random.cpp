#include "Random.h"

std::random_device   Random::mRd;
std::mt19937         Random::mRng(Random::mRd());
std::uniform_int_distribution<> Random::boolDist(0,1);