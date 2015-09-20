#ifndef GRID_H
#define GRID_H
#include <vector>

class Grid {
    public:
        Grid();

    private:
        std::vector<int> const numbers;
        std::vector<int> const hlines;
        std::vector<int> const vlines;
};

#endif
