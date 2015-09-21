#ifndef GRID_H
#define GRID_H
#include <vector>
#include "enums.h"

class Grid {
    public:
        Grid();
        ~Grid();
        void print();

    private:
        char formatNumber(int i, int j) const;
        char formatHLine(int i, int j) const;
        char formatVLine(int i, int j) const;

        int m = 10;     // number of rows
        int n = 10;     // number of columns
        Number ** numbers;
        Edge ** hlines;
        Edge ** vlines;
};

#endif
