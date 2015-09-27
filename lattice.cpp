#include "lattice.h"
#include <iostream>
#include <string>
#include "enums.h"

#define POINT '.'
#define HLINE '-'
#define VLINE '|'
#define EX 'x'
#define BLANK ' '

Lattice::Lattice() { }

Lattice::~Lattice() {
    if (init_) {
        for (int i = 0; i < m_; i++) {
            delete [] numbers_[i], hlines_[i], vlines_[i];
        }
        // hlines_ needs one extra
        delete [] hlines_[m_];
        // and delete the outer arrays
        delete [] numbers_, hlines_, vlines_;
    }
}

void Lattice::initArrays(int m, int n) {
    init_ = true;

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

Number Lattice::getNumber(int i, int j) {
    return numbers_[i][j];
}

void Lattice::setHLine(int i, int j, Edge edge) {
    hlines_[i][j] = edge;
}

Edge Lattice::getHLine(int i, int j) {
    return hlines_[i][j];
}

void Lattice::setVLine(int i, int j, Edge edge) {
    vlines_[i][j] = edge;
}

Edge Lattice::getVLine(int i, int j) {
    return vlines_[i][j];
}
