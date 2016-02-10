#ifndef GENERATOR_H
#define GENERATOR_H
#include "../shared/grid.h"
#include "../shared/structs.h"
#include "../solver/rule.h"
#include "../solver/contradiction.h"
#include "../solver/solver.h"


#include <stack>

class Generator {
    public:
        Generator(int m, int n);

    private:
        void genPuzzle();

        void reduceNumbers();
    
        void initArrays();
        void destroyArrays();
        void removeNumber(int i, int j);
        void eliminateNumber(int i, int j);
        void findNumberToRemove();
        bool eligible(int i, int j);
        void fillEligibleVector();
        void markEligible(int i, int j);
        void markNecessary(int i, int j);
        void setOldNumber(int i, int j);
    
        bool checkIfSolved();
        
        void getNecessaryCoordinate();
        
        
        int m_;
        int n_;
        int factor_;
        int numberCount_;
        Grid grid_;
        std::vector <Coordinates> eligibleCoordinates_;
        std::vector <Coordinates> ineligibleCoordinates_;
        Rule * rules_;
        
        Contradiction * contradictions_;
        bool ** canEliminate_;
        Number ** oldNumbers_;
};

#endif
