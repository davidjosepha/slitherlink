#ifndef GENERATOR_H
#define GENERATOR_H
#include "../shared/grid.h"
#include "../shared/structs.h"
#include "../solver/rule.h"
#include "../solver/contradiction.h"

#include <stack>

class Generator {
    public:
        Generator(int m, int n);

    private:
        void genPuzzle();

        void reduceNumbers();
    
        void initArrays();
        void destroyArrays();
        void eliminateNumber(int i, int j);
        bool eligible(int i, int j);
        
        int m_;
        int n_;
        int numberCount_;
        Grid grid_;
        Coordinates recentlyNeeded_;
        std::stack <Coordinates> eliminatedCoordinates_;
        Rule * rules_;
        
        Contradiction * contradictions_;
        bool ** canEliminate_;
        Number ** oldNumbers_;
};

#endif
