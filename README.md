# NEAT

Interface with the code is done via "Coordinator.h". "Main.h" and "Test.h" are not ment to exist in a final solution.

### Purpose
Uses a genetic algorithm to train neural networks. All networks being trained have the same number of input and output nodes, which are
defined by "numIn" and "numOut". The amount of networks that are to be trained at once are defined by "numAI".

## Class Descriptions

### Coordinator.h

#### init(int, int, int)
Always call this before doing anything to setup all variables, unless you have loaded a file instead.

#### calcInput(int, float*)
Set input values for the input-nodes for a specific network between index 0 to #networks. Does a topological sorting of nodes and computes output values.

#### float* getOutput(int)
Returns an array with the result of calcInput(...).

#### setFitness(int, int)
Set fitness value for a specific network between index 0 to #networks. Fitness values are used to compute optimal networks, and are decided by an external fitness evaluation function, which is not provided by this code.

#### evolve()
Performs various genetic algorithms to transform the current set of neural networks to a new generation of networks. Some, or all networks may be modified after this function is run. How the new networks appear depends largely on the fitness values.
