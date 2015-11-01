#include "export.h"
#include <iostream>
#include <string>
#include "constants.h"
#include "enums.h"
#include "lattice.h"

/* Constructor taking as input a lattice to be exported */
Export::Export(Lattice const & lattice) {
    lattice_ = &lattice;
}

/* Outputs a lattice to stdout in a human readable format */
void Export::print() {
    int m = lattice_->getHeight();
    int n = lattice_->getWidth();

    for (int i = 1; i < m-1; i++) {
        /* print points/lines/Xs/nothing above the row of numbers */
        for (int j = 1; j < n-1; j++) {
            std::cout << POINT;
            std::cout << ' ';
            std::cout << formatHLine(i, j);
            std::cout << ' ';
        }
        std::cout << POINT << std::endl;

        /* print row of numbers */
        for (int j = 1; j < n-1; j++) {
            /* print line/x/nothing to the left of number */
            std::cout << formatVLine(i, j);
            std::cout << ' ';
            /* print number */
            std::cout << formatNumber(i, j);
            std::cout << ' ';
        }
        /* print line/x/nothing to the right of last number */
        std::cout << formatVLine(i, n-1) << std::endl;
    }

    /* print lines/Xs/nothing below the last row of numbers */
    for (int j = 1; j < n-1; j++) {
        std::cout << POINT;
        std::cout << ' ';
        std::cout << formatHLine(m-1, j);
        std::cout << ' ';
    }
    std::cout << POINT << std::endl;
}

/* Helper function for formatting a value from the Number
 * enumeration into a human readable 0-3 or blank space. */
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

/* Helper function for formatting a value from the Number
 * enumeration into a human readable '-', 'x' or blank space. */
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

/* Helper function for formatting a value from the Number
 * enumeration into a human readable '|', 'x' or blank space. */
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
