#include "lattice.h"
#include <iostream>
#include <string>
#include "enums.h"

#define POINT '.'
#define HLINE '-'
#define VLINE '|'
#define EX 'x'
#define BLANK ' '

Lattice::Lattice() {
    import();
    print();
}

Lattice::~Lattice() {
    for (int i = 0; i < m_; i++) {
        delete [] numbers_[i], hlines_[i], vlines_[i];
    }
    // hlines_ needs one extra
    delete [] hlines_[m_];
    // and delete the outer arrays
    delete [] numbers_, hlines_, vlines_;
}

void Lattice::initArrays(int m, int n) {
    numbers_ = new Number*[m];
    hlines_ = new Edge*[m+1];
    vlines_ = new Edge*[m];
    for (int i = 0; i < m; i++) {
        numbers_[i] = new Number[n];
        hlines_[i] = new Edge[n];
        vlines_[i] = new Edge[n+1];
    }
    // hlines_ needs one extra
    hlines_[m] = new Edge[n];
}

void Lattice::setNumber(int i, int j, Number num) {
    numbers_[i][j] = num;
}

void Lattice::setHLine(int i, int j, Edge edge) {
    hlines_[i][j] = edge;
}

void Lattice::setVLine(int i, int j, Edge edge) {
    vlines_[i][j] = edge;
}

void Lattice::print() {
    for (int i = 0; i < m_; i++) {
        // print points/lines/Xs/nothing above the row of numbers
        for (int j = 0; j < n_; j++) {
            std::cout << POINT;
            std::cout << formatHLine(i, j);
        }
        std::cout << POINT << std::endl;

        // print row of numbers
        for (int j = 0; j < n_; j++) {
            // print line/x/nothing to the left of number
            std::cout << formatVLine(i, j);
            // print number
            std::cout << formatNumber(i, j);
        }
        // print line/x/nothing to the right of last number
        std::cout << formatVLine(i, n_) << std::endl;
    }

    // print lines/Xs/nothing below the last row of numbers
    for (int j = 0; j < n_; j++) {
        std::cout << POINT;
        std::cout << formatHLine(m_, j);
    }
    std::cout << POINT << std::endl;
}

void Lattice::import() {
    std::string buffer;

    // source info
    std::getline(std::cin, buffer);

    // get dimensions
    std::cin >> m_;
    std::cin >> n_;

    // blank lines
    std::getline(std::cin, buffer);
    std::getline(std::cin, buffer);

    // numbers
    for (int i = 0; i < m_; i++) {
        std::getline(std::cin, buffer);
        importNumberRow(i, buffer);
    }

    // blank line
    std::getline(std::cin, buffer);

    // horizontal lines
    for (int i = 0; i < m_+1; i++) {
        std::getline(std::cin, buffer);
        importHLineRow(i, buffer);
    }

    // blank line
    std::getline(std::cin, buffer);

    // vertical lines
    for (int i = 0; i < m_; i++) {
        std::getline(std::cin, buffer);
        importVLineRow(i, buffer);
    }
}

void Lattice::importNumberRow(int line, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '0':
                numbers_[line][j] = ZERO;
                break;
            case '1':
                numbers_[line][j] = ONE;
                break;
            case '2':
                numbers_[line][j] = TWO;
                break;
            case '3':
                numbers_[line][j] = THREE;
                break;
            default:
                numbers_[line][j] = NONE;
                break;
        }
    }
}

void Lattice::importHLineRow(int line, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '-':
                hlines_[line][j] = LINE;
                break;
            case 'x':
                hlines_[line][j] = NLINE;
                break;
            default:
                hlines_[line][j] = EMPTY;
                break;
        }
    }
}

void Lattice::importVLineRow(int line, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '-':
                vlines_[line][j] = LINE;
                break;
            case 'x':
                vlines_[line][j] = NLINE;
                break;
            default:
                vlines_[line][j] = EMPTY;
                break;
        }
    }
}

char Lattice::formatNumber(int i, int j) const {
    switch (numbers_[i][j]) {
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

char Lattice::formatHLine(int i, int j) const {
    switch (hlines_[i][j]) {
        case LINE:
            return HLINE;
        case NLINE:
            return EX;
        default:
            return BLANK;
    }
}

char Lattice::formatVLine(int i, int j) const {
    switch (vlines_[i][j]) {
        case LINE:
            return VLINE;
        case NLINE:
            return EX;
        default:
            return BLANK;
    }
}
