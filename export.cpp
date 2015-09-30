#include "export.h"
#include <iostream>
#include <string>
#include "enums.h"
#include "lattice.h"
#include "symbols.h"

Export::Export(Lattice const & lattice) {
    lattice_ = &lattice;
}

void Export::print() {
    int m = lattice_->getHeight();
    int n = lattice_->getWidth();

    for (int i = 0; i < m; i++) {
        // print points/lines/Xs/nothing above the row of numbers
        for (int j = 0; j < n; j++) {
            std::cout << POINT;
            std::cout << ' ';
            std::cout << formatHLine(i, j);
            std::cout << ' ';
        }
        std::cout << POINT << std::endl;

        // print row of numbers
        for (int j = 0; j < n; j++) {
            // print line/x/nothing to the left of number
            std::cout << formatVLine(i, j);
            std::cout << ' ';
            // print number
            std::cout << formatNumber(i, j);
            std::cout << ' ';
        }
        // print line/x/nothing to the right of last number
        std::cout << formatVLine(i, n) << std::endl;
    }

    // print lines/Xs/nothing below the last row of numbers
    for (int j = 0; j < n; j++) {
        std::cout << POINT;
        std::cout << ' ';
        std::cout << formatHLine(m, j);
        std::cout << ' ';
    }
    std::cout << POINT << std::endl;
}

char Export::formatNumber(int i, int j) const {
    switch (lattice_->getNumber(i, j)) {
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

char Export::formatHLine(int i, int j) const {
    switch (lattice_->getHLine(i, j)) {
        case LINE:
            return HLINE;
        case NLINE:
            return EX;
        default:
            return BLANK;
    }
}

char Export::formatVLine(int i, int j) const {
    switch (lattice_->getVLine(i, j)) {
        case LINE:
            return VLINE;
        case NLINE:
            return EX;
        default:
            return BLANK;
    }
}
