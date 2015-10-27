#include "lattice.h"
#include <cassert>
#include "enums.h"

#define POINT '.'
#define HLINE '-'
#define VLINE '|'
#define EX 'x'
#define BLANK ' '

/* Destructor checks if arrays have been initialized and
 * if so, frees them all. */
Lattice::~Lattice() {
    destroyArrays();
}

/* Initializes the three two dimensional arrays used to
 * represent a lattice, one each for numbers, horizontal
 * lines, and vertical lines. Sets the init_ variable to
 * true so that the destructor knows to free the memory
 * after destroying an instance of the class. */
void Lattice::initArrays(int m, int n) {
    assert(m > 0 && n > 0);

    destroyArrays();

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

/* Get value of number located at coordinates (i, j),
 * where i is on the range [0, m_+1] and j is on the
 * range [0, n_]. */
Number Lattice::getNumber(int i, int j) const {
    assert(0 <= i && i < m_ && 0 <= j && j < n_);

    return numbers_[i][j];
}

/* Get value of horizontal edge located at coordinates
 * (i, j), where i is on the range [0, m_+1] and j is
 * on the range [0, n_]. */
Edge Lattice::getHLine(int i, int j) const {
    assert(0 <= i && i < m_+1 && 0 <= j && j < n_);

    return hlines_[i][j];
}

/* Get value of vertical edge located at coordinates
 * (i, j), where i is on the range [0, m_+1] and j is
 * on the range [0, n_]. */
Edge Lattice::getVLine(int i, int j) const {
    assert(0 <= i && i < m_ && 0 <= j && j < n_+1);

    return vlines_[i][j];
}

/* Set value of number located at coordinates (i, j),
 * where i is on the range [0, m_+1] and j is on the
 * range [0, n_]. */
void Lattice::setNumber(int i, int j, Number num) {
    assert(0 <= i && i < m_ && 0 <= j && j < n_);

    numbers_[i][j] = num;
}

/* Set value of horizontal edge located at coordinates
 * (i, j), where i is on the range [0, m_+1] and j is
 * on the range [0, n_]. */
bool Lattice::setHLine(int i, int j, Edge edge) { 
    assert(0 <= i && i < m_+1 && 0 <= j && j < n_);

    Edge prevEdge = getHLine(i, j);

    if (prevEdge == EMPTY) {
        hlines_[i][j] = edge;
    } else if (prevEdge != edge) {
        return false;
    }

    return true;
}

/* Set value of vertical edge located at coordinates
 * (i, j), where i is on the range [0, m_+1] and j is
 * on the range [0, n_]. */
bool Lattice::setVLine(int i, int j, Edge edge) { 
    assert(0 <= i && i < m_ && 0 <= j && j < n_+1);

    Edge prevEdge = getVLine(i, j);

    if (prevEdge == EMPTY) {
        vlines_[i][j] = edge;
    } else if (prevEdge != edge) {
        return false;
    }

    return true;
}

/* Deallocates any data allocated in the three two
 * dimensional arrays used to represent the lattice */
void Lattice::destroyArrays() {
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
