#pragma once

#ifdef NEATDLL_EXPORTS
#define NEAT_API __declspec(dllexport)
#else
#define NEAT_API __declspec(dllimport)
#endif

extern "C" NEAT_API void init(int inputs, int outputs, int numAI);
extern "C" NEAT_API void processInput(int index, float* inputs);
extern "C" NEAT_API void processInputAllNEATs(float* inputs);
extern "C" NEAT_API float* getOutput(int index);
extern "C" NEAT_API void setFitness(int index, int fitness);
extern "C" NEAT_API void evolve();
extern "C" NEAT_API void save(int index, char* fileID);
extern "C" NEAT_API void load(char* filename, int numAI);