#ifndef GRID_H
#define GRID_H
#include <vector>

class Grid {
    public:
        Grid();
        void print();

    private:
        char formatNumber(int i, int j) const;
        char formatHLine(int i, int j) const;
        char formatVLine(int i, int j) const;

        int m;     // number of rows
        int n;     // number of columns
        std::vector<int> const numbers;
        std::vector<int> const hlines;
        std::vector<int> const vlines;
};

#endif
