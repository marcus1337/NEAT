#include "Mutate.h"
#include "NEAT.h"
#include "Innovator.h"
#include <iterator>
#include "HelperFuncs.h"

typedef std::pair<int, int> par;

bool Mutate::shouldMutate(float chance) {
    float r = Helper::randf(0.f, 100.f);
    if (r > chance)
        return false;
    return true;
}

bool Mutate::isCircle(NEAT& neat, int from, int to) {
    bool result = false;
    Genome gene(from, to); //ATM genes.size() may not reflect innocationNumber for genes
    neat.nodes[from].genomes.insert(gene);
    
    if (Helper::hasLoop(neat)) {
        result = true;
    }

    neat.nodes[from].genomes.erase(gene);
    return result;
}



void Mutate::linkMutate(NEAT& neat) {
    for (auto& a : neat.nodes)
    {
        for (auto& b : neat.nodes) //add edge a --> b
        {
            float r = Helper::randf(0.f, 100.f);
            if (r > mutationrate)
                continue;

            Node& anode = a.second;
            Node& bnode = b.second;
            int from = anode.getID();
            int to = bnode.getID();
            if (anode.getID() != bnode.getID() && !(bnode.getType() == Node::INPUT) && !(anode.getType() == Node::OUTPUT) &&
                !(anode.getType() == Node::INPUT && bnode.getType() == Node::INPUT) &&
                !Helper::mapContains<par, bool>(neat.busyEdges, std::make_pair(from, to))) {
                if (!isCircle(neat, from, to)) {
                    neat.addGene(from, to);
                }
            }

        }
    }


}


void Mutate::nodeMutate(NEAT& neat) {

    float r = Helper::randf(0.f, 100.f);
    if (r > mutationrate)
       return;

    int randEdge = Helper::randi(0, neat.gencopies.size() - 1);
    neat.gencopies[randEdge].enabled = false;
    neat.gencopies[randEdge].childNodes++;
    neat.updateGene(neat.gencopies[randEdge]);

    Node node(Innovator::getInstance().getNewNodeNum(neat.gencopies[randEdge].getFrom(), neat.gencopies[randEdge].getTo(), neat.gencopies[randEdge].childNodes));
    
    neat.nodes[node.getID()] = node;
    neat.addGene(neat.gencopies[randEdge].getFrom(), node.getID());
    neat.addGene(node.getID(), neat.gencopies[randEdge].getTo());

    /*if (Helper::hasLoop(neat)) {
        std::cout << "HMM " << neat.nodes.size() << " " << neat.gencopies.size() << std::endl;
        abort();
    }*/

}


void Mutate::enableDisableMutate(NEAT& neat) {
    for (size_t i = 0; i < neat.gencopies.size(); i++) {
        if (!shouldMutate(mutationrate))
            continue;

        neat.gencopies[i].enabled = !neat.gencopies[i].enabled;
        neat.updateGene(neat.gencopies[i]);
    }
}

void Mutate::pointMutate(NEAT& neat) {
    for (size_t i = 0; i < neat.gencopies.size(); i++) {
        if (!shouldMutate(mutationrate))
            continue;
        
        if (shouldMutate(0.07f)) {
            neat.gencopies[i].weight = Helper::randf(-2.f, 2.f);
        }
        else {

            neat.gencopies[i].weight += Helper::randf(-0.3f, 0.3f);

            if (neat.gencopies[i].weight > 2.f)
                neat.gencopies[i].weight = 2.f;
            if (neat.gencopies[i].weight < -2.f)
                neat.gencopies[i].weight = -2.f;
        }

        neat.updateGene(neat.gencopies[i]);
    }
}


void Mutate::allMutations(NEAT& neat) {
    //enableDisableMutate(neat);
    //pointMutate(neat);
    
    linkMutate(neat);
   
  //  std::cout << "BEFORE " << Helper::hasLoop(neat) << "\n";

    /*if (Helper::hasLoop(neat)) {
        std::cout << "BEFORE " << std::endl;
        abort();
    }*/

     nodeMutate(neat);
     /*if (Helper::hasLoop(neat)) {
         std::cout << "AFTER " << neat.nodes.size() << " " << neat.gencopies.size() << std::endl;
         abort();
     }*/
    // std::cout << "AFTER " << Helper::hasLoop(neat) << "\n";
}