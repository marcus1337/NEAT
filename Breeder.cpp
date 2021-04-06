
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
    //for (auto& gene : parent1->recurrentGeneCopies)
    //    child.addRecurrentGene(gene);
}

void Breeder::breedFitnessBased(std::vector<std::future<void>>& futures, int numKids) {
    int produced = 0;
    while (numKids > 0) {
        int childIndex = getChildIndex(numKids);
        futures.push_back(std::async(std::launch::async | std::launch::deferred,
            std::bind(&Breeder::breedChild, *this, selectedPairs[produced].first,
                selectedPairs[produced].second, childIndex)));
        numKids--;
        produced++;
    }
}

int Breeder::getChildIndex(int numKids) {
    return numChildrenLeft - numKids;
}

void Breeder::breedElitismOfSpecies(std::vector<std::future<void>>& futures, int numKids) {
    int produced = 0;
    while (numKids > 0) {
        futures.push_back(std::async(std::launch::async | std::launch::deferred,
            std::bind(&Breeder::breedElite, *this, getChildIndex(numKids), selectedSingles[produced])));
        numKids--;
        produced++;
    }
}

void Breeder::breedElite(int childIndex, NEAT* neat) {
    (*children)[childIndex] = *neat;
    Mutate::allMutations((*children)[childIndex]);
}

void Breeder::breedChild(NEAT* g1, NEAT* g2, int childIndex) {
    NEAT child;
    crossOver(child, g1, g2);
    if(child.getNumHiddenNodes() <= Mutate::maxHiddenNodes)
        (*children)[childIndex] = child;
    else {
        if (Utils::randomBool())
            (*children)[childIndex] = *g1;
        else
            (*children)[childIndex] = *g2;
    }
    Mutate::allMutations((*children)[childIndex]);
}

void Breeder::crossOver(NEAT& child, NEAT* n1, NEAT* n2) {
    child.numIn = numIn;
    child.numOut = numOut;
    child.gencopies.reserve(std::max(n1->gencopies.size(), n2->gencopies.size()));

    inheritNodesFromParents(child, n1, n2);
    inheritGenesFromParents(child, n1, n2);
    removeEmptyNodes(child);
}

void NTE::Breeder::removeEmptyNodes(NTE::NEAT & child)
{
    auto iter = child.nodes.begin();
    auto endIter = child.nodes.end();
    for (; iter != endIter; iter)
    {
        if (iter->second.genomes.empty())
            iter = child.nodes.erase(iter);
        else
            ++iter;
    }
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