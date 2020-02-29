#pragma once

class Genome {
    int from, to, id;

public:

    bool enabled;
    float weight;
    int childNodes = 0; //How many times a gene has been split into two new genes + node.

    int getFrom() const;
    int getTo() const;
    int getID() const;
    Genome();
    Genome(int _from, int _to);
    Genome(int _from, int _to, bool _enabled, float _weight, int _childNodes);

    bool operator < (const Genome &right) const;
    bool operator==(const Genome& rhs) const;

};