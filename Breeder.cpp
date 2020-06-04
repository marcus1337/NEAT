
#include "Breeder.h"
#include "Utils.h"
#include "Mutate.h"

using namespace NTE;

void Breeder::incrementIDIndexes(int& i, int& j, int ID1, int ID2) {
    if (ID1 == ID2) {
        i++;
        j++;
    }
    else if (ID1 < ID2)
        i++;
    else
        j++;
}


void Breeder::addRemainingGenesToNeat(NEAT& _neat, int fromIndex, std::vector<Genome>& genes) {
    for (int i = fromIndex; i < genes.size(); i++)
        _neat.addGene(genes[i]);
}

void Breeder::childFromUnequalParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2) {
    int i = 0; int j = 0;
    while (i != g1.size() - 1 && j != g2.size() - 1) {
        if (g1[i].getID() == g2[j].getID())
            addGeneRandomly(child, g1[i], g2[j]);
        if (g1[i].getID() != g2[j].getID())
            child.addGene(g1[i]);
        incrementIDIndexes(i, j, g1[i].getID(), g2[j].getID());
    }
    addRemainingGenesToNeat(child, i, g1);
}

void Breeder::addGeneRandomly(NEAT& child, Genome gene1, Genome gene2) {
    Genome genome = gene1;
    if (Utils::randomBool())
        genome = gene2;
    genome.childNodes = std::max(gene1.childNodes, gene2.childNodes);
    child.addGene(genome);
}

void Breeder::inheritNodesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2) {
    for (const auto &[key, value] : parent1->nodes) {
        Node nod(value.getID(), value.getType());
        child.nodes[nod.getID()] = nod;
    }
    for (const auto &[key, value] : parent2->nodes) {
        Node nod(value.getID(), value.getType());
        child.nodes[nod.getID()] = nod;
    }
}

void Breeder::inheritGenesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2) {
    if (parent1->fitness == parent2->fitness && Utils::randomBool())
        Utils::swap<NEAT*>(parent1, parent2);
    if (parent2->fitness > parent1->fitness)
        Utils::swap<NEAT*>(parent1, parent2);

    childFromUnequalParents(child, parent1->gencopies, parent2->gencopies);
}

int Breeder::calcNumBreeds(const Specie& specie, int totAvg) {
    return specie.getSpecieStrength(specieNum, totAvg)*(specie.neats.size() / 2);
}

int Breeder::totalAvgFit() {
    int total = 0;
    for (const auto& s : *pool)
        total += s.averageFitness;
    return total;
}

void Breeder::breedFitnessBased(std::vector<std::future<void>>& futures, int numKids) {
    int totAvg = totalAvgFit();
    int minNumBreeds = 0;
    while (numKids > 0) {
        int produced = 0;
        for (Specie& spec : *pool) {
            int numBreeds = std::max(calcNumBreeds(spec, totAvg), minNumBreeds);
            produced += numBreeds;
            for (int i = 0; i < numBreeds && numKids > 0; i++) {
                int childIndex = getChildIndex(numKids);
                futures.push_back(std::async(std::launch::async | std::launch::deferred,
                    std::bind(&Breeder::breedChild, *this, spec, childIndex)));
                numKids--;
            }
        }
        if (!produced)
            minNumBreeds++;
    }
}

int Breeder::getChildIndex(int numKids) {
    return numChildrenLeft - numKids;
}

void Breeder::breedElitismOfSpecies(std::vector<std::future<void>>& futures, int numKids) {
    while (numKids > 0) {
        futures.push_back(std::async(std::launch::async | std::launch::deferred,
            std::bind(&Breeder::breedElite, *this, getChildIndex(numKids))));
        numKids--;
    }
}

void Breeder::breedElite(int childIndex) {
    int index = Utils::randi(0, (*pool).size() - 1);
    Specie& spec = (*pool)[index];
    (*children)[childIndex] = (*spec.neats[0]);
    Mutate::allMutations((*children)[childIndex]);
}

void Breeder::breedChild(Specie& specie, int childIndex) {
    NEAT* g1 = specie.getRandomNeat();
    NEAT* g2 = specie.getRandomNeat();
    NEAT child;
    crossOver(child, g1, g2);
    (*children)[childIndex] = child;
    Mutate::allMutations((*children)[childIndex]);
}

void Breeder::crossOver(NEAT& child, NEAT* n1, NEAT* n2) {
    child.numIn = numIn;
    child.numOut = numOut;
    child.gencopies.reserve(std::max(n1->gencopies.size(), n2->gencopies.size()));

    inheritNodesFromParents(child, n1, n2);
    inheritGenesFromParents(child, n1, n2);
}

void Breeder::newGeneration(std::vector<NEAT>& neats, std::vector<NEAT>& oldNeats, std::vector<Specie>& _pool) {
    children = &oldNeats;
    numAI = neats.size();
    numIn = neats[0].numIn;
    numOut = neats[0].numOut;
    numChildrenLeft = numAI;
    pool = &_pool;

    std::vector<std::future<void>> futures;
    futures.reserve(numAI);
    newGeneration(futures);
    for (auto &fut : futures)
        fut.wait();
}

void Breeder::newGeneration(std::vector<std::future<void>>& futures) {
    breedFitnessBased(futures, numChildrenLeft / 2);
    numChildrenLeft -= numChildrenLeft / 2;
    breedElitismOfSpecies(futures, numChildrenLeft);
}