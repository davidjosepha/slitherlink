#ifndef GENERATOR_H
#define GENERATOR_H
#include "../shared/grid.h"
#include "../solver/rule.h"
#include "../solver/contradiction.h"

class Generator {
    public:
        Generator(int m, int n);

    private:
        void genPuzzle();

        void reduceNumbers();
    
        void initArray();
        void destroyArray();

        int m_;
        int n_;
        Grid grid_;
        Rule * rules_;
        Contradiction * contradictions_;
        bool ** canEliminate_;
};

#endif
