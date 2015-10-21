#ifndef SOLVER_H
#define SOLVER_H
#include "enums.h"
#include "grid.h"
#include "rule.h"
#include "contradiction.h"

#define NUM_RULES 34
#define NUM_CONTS 11

class Solver {
    public:
        Solver(Grid & grid, int depth);

    private:
        void solve();
        void makeGuesses();

        void intersectGrids(Grid const & lineGuess, Grid const & nLineGuess);

        void initRules();
        void applyRules();
        void applyRule(int i, int j, Rule & rule, Orientation orient);
        bool ruleApplies(int i, int j, Rule & rule, Orientation orient);

        void initContradictions();
        bool contradictionApplies(int i, int j, Contradiction & contradiction, Orientation orient);
        bool testContradictions();

        Grid * grid_;
        int depth_;
        Rule rules_[NUM_RULES];
        Lattice beforeLattices_[NUM_RULES];
        Lattice afterLattices_[NUM_RULES];
        Contradiction contradictions_[NUM_CONTS];
        Lattice contLattices_[NUM_CONTS];

};

#endif
