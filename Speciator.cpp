#include "Speciator.h"
#include <iostream>
#include "Mutate.h"
#include <algorithm>
#include "Innovator.h"
#include "Utils.h"
using namespace NTE;
void Speciator::init(int _numIn, int _numOut, int _numAI) {
    numOut = _numOut;
    numIn = _numIn;
    numAI = _numAI;
}

void Speciator::prepareForNewGeneration(std::vector<NEAT>& neats) {
    numChildrenLeft = numAI;
    specieNum = 0;
    pool.clear();
    for (size_t i = 0; i < neats.size(); i++)
        std::sort(neats[i].gencopies.begin(), neats[i].gencopies.end());
    createSpecies(neats);
    adjustDynamicSpecieDelta();
}


void Speciator::adjustFitnessShared(std::vector<NEAT>& neats, int index) {
    int divisor = 1;

    for (int i = 0; i < numAI; i++) {
        if (i == index)
            continue;
        if (sameSpecie(neats[i], neats[index]))
            divisor++;
    }
    neats[index].fitness /= divisor;
}

void Speciator::createSpecies(std::vector<NEAT>& neats) {
    for (size_t i = 0; i < neats.size(); i++)
        addToSpecies(neats[i]);
    fitnessSharing(neats);
    preparePool();
}

void Speciator::adjustDynamicSpecieDelta() {
    if (specieNum < targetNumSpecies) {
        speciateDelta -= speciateDelta / 20;
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
    return specie.getSpecieStrength(specieNum, totAvg)*(specie.neats.size() / 2);
}

void Speciator::addRemainingGenesToNeat(NEAT& _neat, int fromIndex, std::vector<Genome>& genes) {
    for (int i = fromIndex; i < genes.size(); i++)
        _neat.addGene(genes[i]);
}

void Speciator::addRemainingGenesToNeatRandomly(NEAT& _neat, int fromIndex, std::vector<Genome>& genes) {
    for (int i = fromIndex; i < genes.size(); i++) {
        if (Utils::randomBool())
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

void Speciator::childFromUnequalParents(NEAT& child, std::vector<Genome>& g1, std::vector<Genome>& g2) {
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

void Speciator::addGeneRandomly(NEAT& child, Genome gene1, Genome gene2) {
    Genome genome = gene1;
    if (Utils::randomBool())
        genome = gene2;
    genome.childNodes = std::max(gene1.childNodes, gene2.childNodes);
    child.addGeneNoLoop(genome);
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
    if (parent1->fitness == parent2->fitness && Utils::randomBool())
        Utils::swap<NEAT*>(parent1, parent2);
    if (parent2->fitness > parent1->fitness)
        Utils::swap<NEAT*>(parent1, parent2);

    childFromUnequalParents(child, parent1->gencopies, parent2->gencopies);
}

bool Speciator::isWeak(const Specie& specie, int numSpecies, int totalAverageFitness) {
    return specie.getSpecieStrength(numSpecies, totalAverageFitness) < 0.8f;
}

void Speciator::removeWeakSpecies() {
    totAvg = totalAvgFit();
    Specie backupSpecie = pool[0];
    pool.erase(std::remove_if(pool.begin(), pool.end(),
        [&](const Specie& o) { return isWeak(o, specieNum, totAvg); }), pool.end());
    if (pool.empty())
        pool.push_back(backupSpecie);
}

void Speciator::breedFitnessBased(std::vector<std::future<void>>& futures, int numKids) {
    totAvg = totalAvgFit();
    int minNumBreeds = 0;
    while (numKids > 0) {
        int produced = 0;
        for (Specie& spec : pool) {
            int numBreeds = std::max(calcNumBreeds(spec), minNumBreeds);
            produced += numBreeds;
            for (int i = 0; i < numBreeds && numKids > 0; i++) {
                int childIndex = getChildIndex(numKids);
                futures.push_back(std::async(std::launch::async | std::launch::deferred,
                    std::bind(&Speciator::breedChild, *this, spec, childIndex)));
                numKids--;
            }
        }
        if (!produced)
            minNumBreeds++;
    }
}

int Speciator::getChildIndex(int numKids) {
    return numChildrenLeft - numKids;
}

void Speciator::breedElitismOfSpecies(std::vector<std::future<void>>& futures, int numKids) {
    while (numKids > 0) {
        futures.push_back(std::async(std::launch::async | std::launch::deferred,
            std::bind(&Speciator::breedElite, *this, getChildIndex(numKids))));
        numKids--;
    }
}

void Speciator::breedElite(int childIndex) {
    int index = Utils::randi(0, pool.size() - 1);
    Specie& spec = pool[index];
    (*children)[childIndex] = (*spec.neats[0]);
    Mutate::allMutations((*children)[childIndex]);
}

void Speciator::breedChild(Specie& specie, int childIndex) {
    NEAT* g1 = specie.getRandomNeat();
    NEAT* g2 = specie.getRandomNeat();
    if (Utils::randi(0, 100) > Speciator::crossChance) {
        NEAT child(*g1);
        (*children)[childIndex] = child;
    }
    else {
        NEAT child;
        crossOver(child, g1, g2);
        (*children)[childIndex] = child;
    }
    Mutate::allMutations((*children)[childIndex]);
}

void Speciator::crossOver(NEAT& child, NEAT* n1, NEAT* n2) {
    child.numIn = numIn;
    child.numOut = numOut;
    child.gencopies.reserve(std::max(n1->gencopies.size(), n2->gencopies.size()));

    inheritNodesFromParents(child, n1, n2);
    inheritGenesFromParents(child, n1, n2);
}

void Speciator::newGeneration() {
    std::vector<std::future<void>> futures;
    futures.reserve(numAI);
    newGeneration(futures);
    for (auto &fut : futures)
        fut.wait();
}

void Speciator::removeStaleSpecies() {
    if (pool.size() >= numSpeciesLimit)
        pool.erase(pool.begin() + numSpeciesLimit, pool.end());
}

void Speciator::preparePool() {
    for (Specie& spec : pool)
        for (const auto& neat : spec.neats)
            spec.topFitness = std::max(spec.topFitness, neat->fitness);
    std::sort(pool.begin(), pool.end(), [](const Specie& lhs, const Specie& rhs)
    { return lhs.topFitness > rhs.topFitness; });
    for (Specie& spec : pool) {
        sortSpecie(spec);
        spec.calcAvgFit();
    }
}

void Speciator::sortSpecie(Specie& spec) {
    std::sort(spec.neats.begin(), spec.neats.end(), [](const NEAT* lhs, const NEAT* rhs)
    {
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
        NEAT& tmpNeat = *pool[i].neats[Utils::randi(0, numNeatsInSpecie - 1)];
        if (sameSpecie(neat, tmpNeat)) {
            pool[i].neats.push_back(&neat);
            return true;
        }
    }
    return false;
}

void Speciator::addNewSpecie(NEAT& neat) {
    specieNum++;
    Specie spec(specieNum);
    spec.neats.push_back(&neat);
    pool.push_back(spec);
}

bool Speciator::sameSpecie(NEAT& n1, NEAT& n2) {
    float dd = c1 * disjointDiff(n1.gencopies, n2.gencopies);
    float dw = c2 * weightDiff(n1.gencopies, n2.gencopies);
    float de = c3 * excessDiff(n1.gencopies, n2.gencopies);
    return (dd + dw + de) < speciateDelta;
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
    int N = std::max<int>(g1.size(), g2.size());
    int i = 0, j = 0;
    while (i != g1.size() - 1 && j != g2.size() - 1) {
        if (g1[i].getID() != g2[j].getID() && IDInRange(g1[i].getID(), g2) && IDInRange(g2[j].getID(), g1))
            res++;
        incrementIDIndexes(i, j, g1[i].getID(), g2[j].getID());
    }
    return res / N;
}

float Speciator::excessDiff(std::vector<Genome>& g1, std::vector<Genome>& g2) {
    float res = 0;
    int N = std::max<int>(g1.size(), g2.size());
    for (int i = 0; i < g1.size(); i++) {
        if (!IDInRange(g1[i].getID(), g2))
            res++;
    }
    for (int i = 0; i < g2.size(); i++) {
        if (!IDInRange(g2[i].getID(), g1))
            res++;
    }
    return res / N;
}

bool Speciator::IDInRange(int _id, std::vector<Genome>& genomeArr) {
    int minID = genomeArr[0].getID();
    int maxID = genomeArr[genomeArr.size() - 1].getID();
    return _id >= minID && _id <= maxID;
}

void Speciator::fitnessSharing(std::vector<NEAT>& neats) {
    for (int i = 0; i < numAI; i++)
        adjustFitnessShared(neats, i);
}

void Speciator::newGeneration(std::vector<std::future<void>>& futures) {
    breedFitnessBased(futures, numChildrenLeft / 2);
    numChildrenLeft -= numChildrenLeft / 2;
    breedElitismOfSpecies(futures, numChildrenLeft);
}

void Speciator::cullSpecies() {
    removeStaleSpecies();
    removeWeaksInSpecies();
    removeWeakSpecies();
}