#ifndef SOLVER_H
#define SOLVER_H
#include "constants.h"
#include "contradiction.h"
#include "enums.h"
#include "epq.h"
#include "grid.h"
#include "rule.h"

class Solver {
    public:
        Solver(Grid & grid, Rule rules[NUM_RULES], Contradiction contradictions[NUM_CONTRADICTIONS], int depth);

    private:
        void solve();
        void solveDepth(int depth);
        void makeHLineGuess(int i, int j, int depth);
        void makeVLineGuess(int i, int j, int depth);

        bool spiralNext(int startm, int startn, int & prevm, int & prevn) const;

        void intersectGrids(Grid const & lineGuess, Grid const & nLineGuess);

        void applyRules();
        void applyRule(int i, int j, Rule & rule, Orientation orient);
        bool ruleApplies(int i, int j, Rule const & rule, Orientation orient) const;
        
        bool testContradictions() const;
        bool contradictionApplies(int i, int j, Contradiction const & contradiction, Orientation orient) const;

        Grid * grid_;
        int depth_;
        Rule * rules_;
        Contradiction * contradictions_;
        EPQ epq_;
};

#endif
