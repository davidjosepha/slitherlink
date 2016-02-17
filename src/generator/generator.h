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
    
// <<<<<<< HEAD
        // Sam's changes
        void deleteNumbers();
        bool isBalanced(int i, int j, Number num);
        void setCounts();
        void changeCounts(Number num);
// =======
        // Dan's changes (distinct way to code a generator)
        void fillSingleEligible();
        void markEligible(int i, int j);
        void markNecessary(int i, int j);
        void setOldNumber(int i, int j);
    
        // Combining Dan and Sam's Respective Algorithms
        void fillEligibleVectors();
        void findZeroToRemove();
        void findOneToRemove();
        void findTwoToRemove();
        void findThreeToRemove();
        
    
        bool checkIfSolved();
        
        void getNecessaryCoordinate();
        
// >>>>>>> fd3cbc36c9cfe95ac544eafb21161d02b1d09cb1
        
        int m_;
        int n_;
        double factor_;
        int numberCount_;
        int oneCount_;
        int twoCount_;
        int threeCount_;
        int * selectedRules_;
        Grid grid_;
        std::vector <Coordinates> eligibleCoordinates_;
        std::vector <Coordinates> eligibleZeroCoordinates_;
        std::vector <Coordinates> eligibleOneCoordinates_;
        std::vector <Coordinates> eligibleTwoCoordinates_;
        std::vector <Coordinates> eligibleThreeCoordinates_;
    
        std::vector <Coordinates> ineligibleCoordinates_;
        Rule * rules_;
        Contradiction * contradictions_;
        bool ** canEliminate_;
        Number ** oldNumbers_;

};

#endif
