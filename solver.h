#ifndef SOLVER_H
#define SOLVER_H
#include "lattice.h"
#include "rule.h"

class Solver {
    public:
        Solver(Lattice & grid);

    private:
        void applyRules();
        void applyRule(int i, int j, Rule & rule);
        Lattice * grid_;
};

#endif
