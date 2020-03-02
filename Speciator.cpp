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
    specieNum = 0;
    pool.clear();
    children.clear();
    for (size_t i = 0; i < neats.size(); i++)
        addToSpecies(neats[i]);
    sortPoolAndSpecies();
}

void Speciator::speciate(std::vector<NEAT>& neats) {
    prepareForNewGeneration(neats);
    removeStaleSpecies();
    cullSpecies();
    newGeneration();
    neats = std::vector<NEAT>(children);
}

int Speciator::totalAvgFit() {
    int total = 0;
    for (const auto& s : pool) {
        total += s.averageFitness;
    }
    return total;
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
        if (Utils::randomBool())
            _neat.addGene(gene1.getFrom(), gene1.getTo());
        else
            _neat.addGene(gene2.getFrom(), gene2.getTo());
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
        incrementIDIndexes(i, j, g1[i].getID(), g2[j].getID());
    }
    addRemainingGenesToNeat(child, j, g2);
}

NEAT Speciator::makeChildWithoutGenes(NEAT* parent1, NEAT* parent2) {
    NEAT child(numIn, numOut);
    for (const auto &[key, value] : parent1->nodes) {
        Node nod(value.getID(), value.getType());
        child.nodes[nod.getID()] = nod;
    }
    for (const auto &[key, value] : parent2->nodes) {
        Node nod(value.getID(), value.getType());
        child.nodes[nod.getID()] = nod;
    }
    return child;
}

void Speciator::inheritGenesFromParents(NEAT& child, NEAT* parent1, NEAT* parent2) {
    if (parent1->fitness == parent2->fitness)
        childFromEqualParents(child, parent1->gencopies, parent2->gencopies);
    else {
        if (parent2->fitness > parent1->fitness)
            Utils::swap<NEAT*>(parent1, parent2);
        child.copyPointer(parent1);
        childFromUnequalParents(child, parent1->gencopies, parent2->gencopies);
    }
    child.removeRedundants();
}

void Speciator::crossOver(NEAT* n1, NEAT* n2) {
    NEAT child = makeChildWithoutGenes(n1, n2);
    inheritGenesFromParents(child, n1, n2);
    Mutate::allMutations(child);
    children.push_back(child);
}

void Speciator::breedChild(Specie& specie) {
    NEAT* g1 = specie.getRandomNeat();
    NEAT* g2 = specie.getRandomNeat();

    if (Utils::randf(0, 100) > crossChance) {
        NEAT child(*g1);
        Mutate::allMutations(child);
        children.push_back(child);
    }
    else
        crossOver(g1, g2);
}

bool Speciator::isWeak(const Specie& o) {
    int numBreeds = (int)(((float)o.averageFitness / (float)totAvg)*(float)numAI);
    return numBreeds < 1;
}

void Speciator::removeWeakSpecies() {
    pool.erase(std::remove_if(pool.begin(), pool.end(),
        [=](const Specie& o) {
        return this->isWeak(o);
    }), pool.end());
}


void Speciator::newGeneration() {

    for (Specie& spec : pool) {
        spec.calcAvgFit();
    }
    totAvg = totalAvgFit();

    removeWeakSpecies();

    for (Specie& spec : pool) {
        if (children.size() == numAI)
            return;

        int numBreeds = (int)(((float)spec.averageFitness / (float)totAvg)*(float)numAI) - 1;
        for (int i = 0; i < numBreeds; i++) {
            breedChild(spec);
            if (children.size() == numAI)
                return;
        }
    }

    cullAllButOneFromSpecies();

    while (children.size() < numAI) {
        int index = Utils::randi(0, pool.size() - 1);
        Specie& spec = pool[index];
        breedChild(spec);
    }
}

void Speciator::removeStaleSpecies() {
    std::vector<Specie> survived;
    for (size_t i = 0; i < pool.size() && i < 200; i++) {
        survived.push_back(pool[i]);
    }
    pool = survived;
}

void Speciator::sortPoolAndSpecies() {
    std::sort(pool.begin(), pool.end(), [](const Specie& lhs, const Specie& rhs)
    {
        return lhs.topFitness > rhs.topFitness;
    });
    for (Specie& spec : pool)
        std::sort(spec.neats.begin(), spec.neats.end(), [](const NEAT* lhs, const NEAT* rhs)
        {   //smallest fitness in beginning
        return lhs->fitness > rhs->fitness;
        });
}

void Speciator::cullSpecies() {
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
        NEAT& tmpNeat = *pool[i].neats[0];
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
    return (dd + dw) < 1.0f;
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
