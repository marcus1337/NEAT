#include "Genome.h"
#include "Utils.h"
#include "Innovator.h"

int Genome::getFrom() const {
    return from;
}

int Genome::getTo() const {
    return to;
}

int Genome::getID() const {
    return id;
}

Genome::Genome() : id(-1) {}

Genome::Genome(int _from, int _to) :
    from(_from), to(_to), id(Innovator::getInstance().getNum(_from, _to)), enabled(true) {
    weight = Utils::randf(-2.f, 2.f);
}

Genome::Genome(int _from, int _to, bool _enabled, float _weight, int _childNodes) :
    from(_from), to(_to), id(Innovator::getInstance().getNum(_from, _to)), 
    enabled(_enabled), weight(_weight), childNodes(_childNodes) {}


bool Genome::operator < (const Genome &right) const
{
    return id < right.id;
}
bool Genome::operator==(const Genome& rhs) const { return this->id == rhs.id; }