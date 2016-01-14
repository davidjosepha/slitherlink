#ifndef LOOPGEN_H
#define LOOPGEN_H
#include "../solver/grid.h"
#include "../solver/enums.h"

class LoopGen {
    public:
        LoopGen(int m, int n, Grid grid);

    private:
        void genLoop();
        LoopCell getLoopCell(int i, int j, LoopCell ** loop) const;

        Grid grid_;
        int m_;
        int n_;
};

#endif
