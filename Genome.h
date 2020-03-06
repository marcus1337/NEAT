#ifndef GENOME_H
#define GENOME_H

class Genome {
    int from, to, id;

public:

    mutable bool enabled;
    mutable float weight;
    mutable int childNodes = 0; //How many times a gene has been split into two new genes + node.

    int getFrom() const;
    int getTo() const;
    int getID() const;

    static Genome dummyGenome(int _from, int _to);
    Genome();
    Genome(int _from, int _to);
    Genome(int _from, int _to, bool _enabled, float _weight, int _childNodes);

    bool operator < (const Genome &right) const;
    bool operator==(const Genome& rhs) const;

};

#endif