#ifndef RULE_H
#define RULE_H
#include "enums.h"
#include "lattice.h"

class Rule {
    public:
        Rule();

    private:
        Lattice before;
        Lattice after;
};

#endif
