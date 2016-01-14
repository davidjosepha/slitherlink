#ifndef GENERATOR_H
#define GENERATOR_H
#include "../solver/grid.h"

class Generator {
    public:
        Generator(int m, int n);

    private:
        void genPuzzle();

        void reduceNumbers();

        Grid grid_;
        int m_;
        int n_;
};

#endif
