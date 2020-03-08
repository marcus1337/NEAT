# NEAT

Interface with the code is done via "Coordinator.h". "Main.h" and "Test.h" are not ment to exist in a final solution. For performance reasons, the project is intended to be statically linked to Microsofts memory allocator mimalloc (https://github.com/microsoft/mimalloc).

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

### Innovator.h
Provides unique IDs for genomes and nodes.

### Mutate.h
Randomly modifies a network. Possible mutations are:  
a) enable/disable a genome  
b) change weight of a genome  
c) create a new genome  
d) replace a genome by 1 new node connected by 2 new genomes  

### NEAT.h
Stores a neural network, and an array having extra copies of all genomes.

### Utils.h
Various static helper functions, used by most other files.

## Data

### Genome.h
An edge going from node A to node B. Each genome has an ID and weight.

### Node.h
Each node has an ID and a set of Genomes.

### NEATDoubleBuffer.h
Used for computational optimizations. Stores networks in two arrays that are swapped via pointers.

### Specie.h
Used by evolve() to separate sets of networks into groups. The groups are decided by a dynamic evaluation function that compares how different networks are. Are they too different they get split into separate species.


