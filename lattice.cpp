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

Lattice::Lattice(int m, int n) {
    initArrays(m, n);
}

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
    m_ = m;
    n_ = n;

    numbers_ = new Number*[m_];
    hlines_ = new Edge*[m_+1];
    vlines_ = new Edge*[m_];
    for (int i = 0; i < m_; i++) {
        numbers_[i] = new Number[n_];
        hlines_[i] = new Edge[n_];
        vlines_[i] = new Edge[n_+1];
    }
    // hlines_ needs one extra
    hlines_[m_] = new Edge[n_];
}

void Lattice::cleanArrays() {
    if (init_) {
        for (int i = 0; i < m_; i++) {
            for (int j = 0; j < n_; j++) {
                numbers_[i][j] = NONE;
                hlines_[i][j] = EMPTY;
                vlines_[i][j] = EMPTY;
            }
            vlines_[i][n_] = EMPTY;
        }

        for (int j = 0; j < n_; j++) {
            hlines_[m_][j] = EMPTY;
        }
    }
}
