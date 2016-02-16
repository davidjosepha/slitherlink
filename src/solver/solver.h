#ifndef SOLVER_H
#define SOLVER_H
#include "contradiction.h"
#include "epq.h"
#include "rule.h"
#include "../shared/constants.h"
#include "../shared/enums.h"
#include "../shared/grid.h"

class Solver {
    public:
        Solver(Grid & grid, Rule rules[NUM_RULES], Contradiction contradictions[NUM_CONTRADICTIONS], int selectedRules[], int selectLength, int depth);
        Solver(Grid & grid, Rule rules[NUM_RULES], Contradiction contradictions[NUM_CONTRADICTIONS], int selectedRules[], int selectLength, int depth, EPQ oldEPQ);
        bool testContradictions() const;
        bool hasMultipleSolutions() const {return multipleSolutions_;};
        void resetSolver();
        int ruleCounts_;

    private:
        void solve();
        void solveDepth(int depth);
        void makeHLineGuess(int i, int j, int depth);
        void makeVLineGuess(int i, int j, int depth);

        void updateEPQ();

        bool spiralNext(int startm, int startn, int & prevm, int & prevn) const;

        void intersectGrids(Grid const & lineGuess, Grid const & nLineGuess);

        void applyRules(int selectedRules[]);
        void applyRule(int i, int j, Rule & rule, Orientation orient);
        bool ruleApplies(int i, int j, Rule const & rule, Orientation orient) const;
        
        
        bool contradictionApplies(int i, int j, Contradiction const & contradiction, Orientation orient) const;

        Grid * grid_;
        int depth_;
        Rule * rules_;
        int * selectedRules_;
        int selectLength_;
        Contradiction * contradictions_;
        EPQ epq_;
        int epqSize_;
        bool multipleSolutions_;
};

#endif
