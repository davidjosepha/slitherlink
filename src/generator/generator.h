#ifndef GENERATOR_H
#define GENERATOR_H
#include "../solver/grid.h"
#include "../solver/rule.h"
#include "../solver/contradiction.h"

class Generator {
    public:
        Generator(int m, int n);

    private:
        void genPuzzle();

        void fillGrid();
        void reduceNumbers();

        Grid grid_;
        Rule * rules_;
        Contradiction * contradictions_;
        int m_;
        int n_;
};

#endif
