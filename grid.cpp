#include "grid.h"
#include <iostream>
#include <string>
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

    import();
    print();
}

Grid::~Grid() {
    for (int i = 0; i < m; i++) {
        delete [] numbers[i], hlines[i], vlines[i];
    }
    // hlines needs one extra
    delete [] hlines[m];
    // and delete the outer arrays
    delete [] numbers, hlines, vlines;
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

void Grid::import() {
    std::string buffer;

    // source info
    std::getline(std::cin, buffer);

    // get dimensions
    std::cin >> m;
    std::cin >> n;

    // blank lines
    std::getline(std::cin, buffer);
    std::getline(std::cin, buffer);

    // numbers
    for (int i = 0; i < m; i++) {
        std::getline(std::cin, buffer);
        importNumberRow(i, buffer);
    }

    // blank line
    std::getline(std::cin, buffer);

    // horizontal lines
    for (int i = 0; i < m+1; i++) {
        std::getline(std::cin, buffer);
        importHLineRow(i, buffer);
    }

    // blank line
    std::getline(std::cin, buffer);

    // vertical lines
    for (int i = 0; i < m; i++) {
        std::getline(std::cin, buffer);
        importVLineRow(i, buffer);
    }
}

void Grid::importNumberRow(int line, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '0':
                numbers[line][j] = ZERO;
                break;
            case '1':
                numbers[line][j] = ONE;
                break;
            case '2':
                numbers[line][j] = TWO;
                break;
            case '3':
                numbers[line][j] = THREE;
                break;
            default:
                numbers[line][j] = NONE;
                break;
        }
    }
}

void Grid::importHLineRow(int line, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '-':
                hlines[line][j] = LINE;
                break;
            case 'x':
                hlines[line][j] = EX;
                break;
            default:
                hlines[line][j] = EMPTY;
                break;
        }
    }
}

void Grid::importVLineRow(int line, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '-':
                vlines[line][j] = LINE;
                break;
            case 'x':
                vlines[line][j] = EX;
                break;
            default:
                vlines[line][j] = EMPTY;
                break;
        }
    }
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
