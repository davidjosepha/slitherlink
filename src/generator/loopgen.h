#ifndef LOOPGEN_H
#define LOOPGEN_H
#include "../solver/grid.h"
#include "../solver/enums.h"

class LoopGen {
    public:
        LoopGen(int m, int n, Grid grid);

    private:
        void genLoop();

        void initArray();
        void destroyArray();

        LoopCell getLoopCell(int i, int j) const;
        Coordinates chooseNext(int i, int j) const;

        LoopCell ** loop_;
        Grid grid_;
        int m_;
        int n_;
};

#endif
