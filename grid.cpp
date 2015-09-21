#include "grid.h"
#include <iostream>
#include <vector>

#define POINT '.'
#define HLINE '-'
#define VLINE '|'

Grid::Grid() {
    m = 10;
    n = 10;
    print();
}

void Grid::print() {
    for (int i = 0; i < m; i++) {
        // print points/lines/Xs/nothing above the row of numbers
        for (int j = 0; j < n; j++) {
            std::cout << POINT;
            std::cout << formatHLine(i, j);
        }
        std::cout << POINT << std::endl;

        // print row of numbers
        for (int j = 0; j < n; j++) {
            // print line/x/nothing to the left of number
            std::cout << formatVLine(i, j);
            // print number
            std::cout << formatNumber(i, j);
        }
        // print line/x/nothing to the right of last number
        std::cout << formatVLine(i, n) << std::endl;
    }

    // print lines/Xs/nothing below the last row of numbers
    for (int j = 0; j < n; j++) {
        std::cout << POINT;
        std::cout << formatHLine(m, j);
    }
    std::cout << POINT << std::endl;
}

char Grid::formatNumber(int i, int j) const {
    return '1';
}

char Grid::formatHLine(int i, int j) const {
    return HLINE;
}

char Grid::formatVLine(int i, int j) const {
    return VLINE;
}
