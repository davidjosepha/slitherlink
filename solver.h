#ifndef SOLVER_H
#define SOLVER_H
#include "lattice.h"
#include "rule.h"

#define NUM_RULES 1

class Solver {
    public:
        Solver(Lattice & grid);

    private:
        void initRules();
        void applyRules();
        void applyRule(int i, int j, Rule & rule);
        bool ruleApplies(int i, int j, Rule & rule);
        Lattice * grid_;
        Rule rules_[NUM_RULES];
        Lattice beforeLattices_[NUM_RULES];
        Lattice afterLattices_[NUM_RULES];
};

#endif
