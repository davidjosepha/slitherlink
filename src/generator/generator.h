#ifndef GENERATOR_H
#define GENERATOR_H
#include "../shared/grid.h"
#include "../shared/structs.h"
#include "../solver/contradiction.h"
#include "../solver/rule.h"
#include "../solver/solver.h"


#include <stack>

class Generator {
    public:
        Generator(int m, int n, Difficulty difficulty);
        

    private:
        void createPuzzle();
        void deleteNumbers();
        void destroyArrays();
        void displayFinalPuzzle();
        void displayPuzzle();
        void fillEligibleVector();
        void initArrays();
    
        void setDifficulty(Difficulty difficulty);

        void reduceNumbers();
    
        
        
        
        void removeNumber(int i, int j);
        void eliminateNumber(int i, int j);
        void findNumberToRemove();
        bool eligible(int i, int j);
        bool isBalanced(int i, int j);
        bool isBalanced(int i, int j, Number num);

    
        void setCounts();
        void minusCounts(Number num);
        
        void markEligible(int i, int j);
        void markNecessary(int i, int j);
        void setOldNumber(int i, int j);
        void setRules(Difficulty difficulty);
    
        
        void plusCounts(Number num);
        
    
        bool checkIfSolved();
        
        void getNecessaryCoordinate();
        
// >>>>>>> fd3cbc36c9cfe95ac544eafb21161d02b1d09cb1
        
        int m_;
        int n_;
        
        int guessDepth_;
        double factor_;
        int numberCount_;
        int smallestCount_;
        int buffer_;
        int bufferReachCount_;
        int zeroCount_;
        int oneCount_;
        int twoCount_;
        int threeCount_;
        int * selectedRules_;
        int numberOfRules_;
        Grid grid_;
        Grid smallestCountGrid_;
        std::vector <Coordinates> eligibleCoordinates_;
    
        std::vector <Coordinates> ineligibleCoordinates_;
        Rule * rules_;
        Contradiction * contradictions_;
        bool ** canEliminate_;
        Number ** oldNumbers_;
        

};

#endif
