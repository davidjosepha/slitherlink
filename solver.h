#ifndef SOLVER_H
#define SOLVER_H
#include "constants.h"
#include "enums.h"
#include "grid.h"
#include "rule.h"
#include "contradiction.h"

#define NUM_CONTS 11

class Solver {
    public:
        Solver(Grid & grid, Rule rules[NUM_RULES], int depth);

    private:
        void solve();
        bool spiralNext(int startm, int startn, int *prevm, int *prevn);
        void solveDepth(int depth);
        void makeHLineGuess(int i, int j, int depth);
        void makeVLineGuess(int i, int j, int depth);

        void intersectGrids(Grid const & lineGuess, Grid const & nLineGuess);

        void applyRules();
        void applyRule(int i, int j, Rule & rule, Orientation orient);
        bool ruleApplies(int i, int j, Rule & rule, Orientation orient);
        

        void initContradictions();
        bool contradictionApplies(int i, int j, Contradiction & contradiction, Orientation orient);
        bool testContradictions();

        Grid * grid_;
        int depth_;
        Rule * rules_;
        Contradiction contradictions_[NUM_CONTS];
        Lattice contLattices_[NUM_CONTS];

};

#endif
