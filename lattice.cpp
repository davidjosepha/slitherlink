#include "lattice.h"
#include <iostream>
#include <string>
#include "enums.h"

#define POINT '.'
#define HLINE '-'
#define VLINE '|'
#define EX 'x'
#define BLANK ' '

/* Destructor checks if arrays have been initialized and
 * if so, frees them all. */
Lattice::~Lattice() {
    if (init_) {
        for (int i = 0; i < m_; i++) {
            delete [] numbers_[i];
            delete [] hlines_[i];
            delete [] vlines_[i];
        }
        // hlines_ needs one extra
        delete [] hlines_[m_];
        // and delete the outer arrays
        delete [] numbers_;
        delete [] hlines_;
        delete [] vlines_;
    }
}

/* Initializes the three two dimensional arrays used to
 * represent a lattice, one each for numbers, horizontal
 * lines, and vertical lines. Sets the init_ variable to
 * true so that the destructor knows to free the memory
 * after destroying an instance of the class. */
void Lattice::initArrays(int m, int n) {
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

    init_ = true;

    cleanArrays();
}

/* Wipes out all data from the three two dimensional
 * arrays so that new data can be added on top of a
 * clean grid. */
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
