#include "Speciator.h"
#include <iostream>
#include "Mutate.h"
#include <algorithm>
#include "Innovator.h"
#include "Utils.h"

Speciator::Speciator() {}

void Speciator::init(int _numIn, int _numOut, int _numAI) {
    numOut = _numOut;
    numIn = _numIn;
    numAI = _numAI;
}

void Speciator::prepareForNewGeneration(std::vector<NEAT>& neats) {
    for (NEAT& neat : neats)
        std::sort(neat.gencopies.begin(), neat.gencopies.end());
    numChildrenLeft = numAI;
    specieNum = 0;
    pool.clear();
    createSpecies(neats);
}

void Speciator::createSpecies(std::vector<NEAT>& neats) {
    for (size_t i = 0; i < neats.size(); i++)
        addToSpecies(neats[i]);
    sortPoolAndSpecies();
    for (Specie& spec : pool)
        spec.calcAvgFit();
    adjustDynamicSpecieDelta();
}

void Speciator::adjustDynamicSpecieDelta() {
    if (specieNum < targetNumSpecies) {
        speciateDelta -= speciateDelta/20;
    }
    if (specieNum > targetNumSpecies) {
        speciateDelta += speciateDelta / 20;
    }
    speciateDelta = std::clamp(speciateDelta, 0.00001f, 1000.0f);
}

void Speciator::speciate(std::vector<NEAT>& neats, std::vector<NEAT>& oldNeats) {
    children = &oldNeats;
    prepareForNewGeneration(neats);
    cullSpecies();
    newGeneration();
}

int Speciator::totalAvgFit() {
    int total = 0;
    for (const auto& s : pool)
        total += s.averageFitness;
    return total;
}

int Speciator::calcNumBreeds(const Specie& specie) {
    return (int)(((float)specie.averageFitness / (float)totAvg)*(float)numAI) - 1;
}

void Speciator::addRemainingGenesToNeat(NEAT& _neat, int fromIndex, std::vector<Genome>& genes) {
    for (int i = fromIndex; i < genes.size(); i++) {
        _neat.addGeneNoLoop(genes[i]);
    }
}

void Speciator::incrementIDIndexes(int& i, int& j, int ID1, int ID2) {
    if (ID1 == ID2) {
        i++;
        j++;
    }
    else if (ID1 < ID2)
        i++;
    else
        j++;
}

void Speciator::addLowestGeneOrRandom(NEAT& _neat, Genome& gene1, Genome& gene2) {
    if (gene1.getID() == gene2.getID()) {
        Genome genome = gene1;
        if (Utils::randomBool())
            genome = gene2;
        genome.childNodes = std::max(gene1.childNodes, gene2.childNodes);
        _neat.addGene(genome);
    }
    else if (gene1.getID() < gene2.getID())
        _neat.addGeneNoLoop(gene1);
    else
        _neat.addGeneNoLoop(gene2);
}

void Speciator::childFromEqualParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2) {
    int i = 0; int j = 0;
    while (i != g1.size() - 1 && j != g2.size() - 1) {
        addLowestGeneOrRandom(child, g1[i], g2[j]);
        incrementIDIndexes(i, j, g1[i].getID(), g2[j].getID());
    }
    addRemainingGenesToNeat(child, j, g2);
    addRemainingGenesToNeat(child, i, g1);
}

void Speciator::childFromUnequalParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2) {
    int i = 0; int j = 0;
    while (i != g1.size() - 1 && j != g2.size() - 1) {
        if (g1[i].getID() > g2[j].getID())
            child.addGeneNoLoop(g2[j]);
        if (g1[i].getID() <= g2[j].getID()) {
            Genome genome = g1[i];
            if (g1[i].getID() == g2[j].getID())
                genome.childNodes = std::max(g1[i].childNodes, g2[j].childNodes);
            child.addGeneNoLoop(genome);
        }
        incrementIDIndexes(i, j, g1[i].getID(), g2[j].getID());
    }
    addRemainingGenesToNeat(child, j, g2);
    addRemainingGenesToNeat(child, i, g1);
}

void Speciator::inheritNodesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2) {
    for (const auto &[key, value] : parent1->nodes) {
        Node nod(value.getID(), value.getType());
        child.nodes[nod.getID()] = nod;
    }
    for (const auto &[key, value] : parent2->nodes) {
        Node nod(value.getID(), value.getType());
        child.nodes[nod.getID()] = nod;
    }
}

void Speciator::inheritGenesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2) {
    if (parent1->fitness == parent2->fitness)
        childFromEqualParents(child, parent1->gencopies, parent2->gencopies);
    else {
        if (parent2->fitness > parent1->fitness)
            Utils::swap<NEAT*>(parent1, parent2);
        childFromUnequalParents(child, parent1->gencopies, parent2->gencopies);
    }
    child.removeRedundants();
}

void Speciator::crossOver(NEAT* n1, NEAT* n2) {
    NEAT child;
    child.numIn = numIn;
    child.numOut = numOut;
    child.gencopies.reserve(std::max(n1->gencopies.size(), n2->gencopies.size()));
    inheritNodesFromParents(child, n1, n2);
    inheritGenesFromParents(child, n1, n2);
    children->push_back(child);
}

void Speciator::breedChild(Specie& specie) {
    NEAT* g1 = specie.getRandomNeat();
    NEAT* g2 = specie.getRandomNeat();
    if (Utils::randi(0, 100) > crossChance) {
        NEAT child(*g1);
        children->push_back(child);
    }
    else
        crossOver(g1, g2);
    Mutate::allMutations((*children)[children->size() - 1]);
}

bool Speciator::isWeak(const Specie& o) {
    return calcNumBreeds(o) < 0;
}

void Speciator::removeWeakSpecies() {
    totAvg = totalAvgFit();
    Specie backupSpecie = pool[0];
    pool.erase(std::remove_if(pool.begin(), pool.end(),
        [&](const Specie& o) { return isWeak(o); }), pool.end());
    if (pool.empty())
        pool.push_back(backupSpecie);
}

void Speciator::breedFitnessBased(int numKids) {
    totAvg = totalAvgFit();
    while (numKids > 0) {
        for (Specie& spec : pool) {
            int numBreeds = calcNumBreeds(spec);
            for (int i = 0; i < numBreeds && numKids > 0; i++) {
                breedChild(spec);
                numKids--;
            }
        }
    }
}

void Speciator::breedElitismOfSpecies(int numKids) {
    while (numKids > 0) {
        int index = Utils::randi(0, pool.size() - 1);
        Specie& spec = pool[index];
        children->push_back(*spec.neats[0]);
        Mutate::allMutations((*children)[children->size() - 1]);
        numKids--;
    }
}

void Speciator::newGeneration() {
    breedFitnessBased(numChildrenLeft/2);
    numChildrenLeft -= numChildrenLeft / 2;
    breedElitismOfSpecies(numChildrenLeft);
}

void Speciator::cullSpecies() {
    removeStaleSpecies();
    removeWeaksInSpecies();
    removeWeakSpecies();
}

void Speciator::removeStaleSpecies() {
    int numSpeciesLimit = 200;
    if (pool.size() >= numSpeciesLimit)
        pool.erase(pool.begin() + numSpeciesLimit, pool.end());
}

void Speciator::sortPoolAndSpecies() {
    std::sort(pool.begin(), pool.end(), [](const Specie& lhs, const Specie& rhs)
    {
        return lhs.topFitness > rhs.topFitness;
    });
    for (Specie& spec : pool)
        std::sort(spec.neats.begin(), spec.neats.end(), [](const NEAT* lhs, const NEAT* rhs)
    {   //low to high
        return lhs->fitness > rhs->fitness;
    });
}

void Speciator::removeWeaksInSpecies() {
    for (Specie& spec : pool) {
        std::vector<NEAT*> survivors;
        int remaining = spec.neats.size() / 2;
        if (remaining == 0)
            survivors.push_back(spec.neats[0]);
        for (int i = 0; i < remaining; i++)
            survivors.push_back(spec.neats[i]);
        spec.neats = survivors;
    }
    pool.erase(std::remove_if(pool.begin(), pool.end(),
        [](const Specie& o) { return o.neats.size() == 0; }), pool.end());
}

void Speciator::cullAllButOneFromSpecies() {
    for (Specie& spec : pool) {
        spec.neats = std::vector<NEAT*>({ spec.neats[0] });
    }
}

void Speciator::addToSpecies(NEAT& neat) {
    bool foundSpecies = addToExistingSpecie(neat);
    if (!foundSpecies)
        addNewSpecie(neat);
}

bool Speciator::addToExistingSpecie(NEAT& neat) {
    for (int i = 0; i < specieNum; i++) {
        int numNeatsInSpecie = pool[i].neats.size();
        NEAT& tmpNeat = *pool[i].neats[Utils::randi(0, numNeatsInSpecie-1)];
        if (sameSpecie(neat, tmpNeat)) {
            pool[i].neats.push_back(&neat);
            if (pool[i].topFitness < neat.fitness)
                pool[i].topFitness = neat.fitness;
            return true;
        }
    }
    return false;
}

void Speciator::addNewSpecie(NEAT& neat) {
    specieNum++;
    Specie spec(specieNum);
    spec.topFitness = neat.fitness;
    spec.neats.push_back(&neat);
    pool.push_back(spec);
}

bool Speciator::sameSpecie(NEAT& n1, NEAT& n2) {
    float dd = c1 * disjointDiff(n1.gencopies, n2.gencopies);
    float dw = c2 * weightDiff(n1.gencopies, n2.gencopies);
    return (dd + dw) < speciateDelta;
}

float Speciator::weightDiff(std::vector<Genome>& g1, std::vector<Genome>& g2) {
    float res = 0;
    float coincident = 0;
    int i = 0, j = 0;
    while (i != g1.size() - 1 && j != g2.size() - 1) {
        if (g1[i].getID() == g2[j].getID()) {
            res += abs(g1[i].weight - g2[j].weight);
            coincident++;
        }
        incrementIDIndexes(i, j, g1[i].getID(), g2[j].getID());
    }
    if (coincident == 0)
        return 0;
    return res / coincident;
}

float Speciator::disjointDiff(std::vector<Genome>& g1, std::vector<Genome>& g2) {
    float res = 0;
    int i = 0, j = 0;
    while (i != g1.size() - 1 && j != g2.size() - 1) {
        if (g1[i].getID() != g2[j].getID())
            res++;
        incrementIDIndexes(i, j, g1[i].getID(), g2[j].getID());
    }
    res += g2.size() - i + g1.size() - j;
    float maxlen = (float)std::max<int>(g1.size(), g2.size());
    return res / maxlen;
}
