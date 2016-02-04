#ifndef LOOPGEN_H
#define LOOPGEN_H
#include <vector>
#include "../shared/enums.h"
#include "../shared/grid.h"
#include "../shared/structs.h"

class LoopGen {
    public:
        LoopGen(int m, int n, Grid & grid);

    private:
        void genLoop();
        void fillGrid();
        int countLines(int i, int j) const;
        bool inLoop(int i, int j) const;

        void initArray();
        void destroyArray();

        Coordinates addCell(Coordinates cur);
        Coordinates pickDirection(Coordinates cur) const;
        bool isExpandable(Coordinates cur);
        AdjacencyList getAdjacent(Coordinates cur) const;
        void addAvailable(Coordinates coords, std::vector<Coordinates> & avail) const;
        Coordinates pickCell(std::vector<Coordinates> & avail) const;
        bool validCell(Coordinates coords, Coordinates cur) const;
        bool cellOpen(int i, int j) const;
        bool inBounds(Coordinates coords) const;

        LoopCell ** loop_;
        Grid * grid_;
        int m_;
        int n_;
};

#endif
