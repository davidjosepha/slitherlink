#ifndef LOOP_H
#define LOOP_H
#include <stack>
#include "../solver/constants.h"
#include "../solver/contradiction.h"
#include "../solver/enums.h"
#include "../solver/grid.h"

class Loop {
    public:
        Loop(Grid & grid, int length, int m, int n);
        
    private:
        void makeLoop();
        bool attemptAdd(int i, int j);
        bool checkUse(int i, int j);
        void unmarkNEdges(int i, int j);
        void clearNEdges();
        Grid * grid_;
        int lengthMin_;
        int length_;
        int m_;
        int n_;
        int endPointI_;
        int endPointJ_;
};

#endif