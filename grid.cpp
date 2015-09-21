#include "grid.h"
#include <iostream>
#include <vector>
#include "enums.h"

#define POINT '.'
#define HLINE '-'
#define VLINE '|'
#define IMPOS 'x'
#define BLANK ' '

Grid::Grid() {
    m = 10;
    n = 10;

    numbers = new Number*[m];
    hlines = new Edge*[m+1];
    vlines = new Edge*[m];
    for (int i = 0; i < m; i++) {
        numbers[i] = new Number[n];
        hlines[i] = new Edge[n];
        vlines[i] = new Edge[n+1];
    }
    // hlines needs one extra
    hlines[m] = new Edge[n];


    numbers[3][5] = THREE;
    vlines[5][4] = LINE;
    hlines[6][3] = LINE;
    vlines[6][3] = EX;

    print();
}

Grid::~Grid() {
    for (int i = 0; i < m; i++) {
        delete [] numbers[i];
        delete [] hlines[i];
        delete [] vlines[i];
    }
    // hlines needs one extra
    delete [] hlines[m];
    // and delete the outer arrays
    delete [] numbers;
    delete [] hlines;
    delete [] vlines;
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
    switch (numbers[i][j]) {
        case ZERO:
            return '0';
        case ONE:
            return '1';
        case TWO:
            return '2';
        case THREE:
            return '3';
        default:
            return BLANK;
    }
}

char Grid::formatHLine(int i, int j) const {
    switch (hlines[i][j]) {
        case LINE:
            return HLINE;
        case EX:
            return IMPOS;
        default:
            return BLANK;
    }
}

char Grid::formatVLine(int i, int j) const {
    switch (vlines[i][j]) {
        case LINE:
            return VLINE;
        case EX:
            return IMPOS;
        default:
            return BLANK;
    }
}
