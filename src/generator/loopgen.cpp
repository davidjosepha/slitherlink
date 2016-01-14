#include "loopgen.h"
#include <stdlib.h>
#include <time.h>
#include "../solver/enums.h"
#include "../solver/grid.h"

/* LoopGen constructor */
LoopGen::LoopGen(int m, int n, Grid grid) {
    m_ = m;
    n_ = n;
    /* instantiate grid */
}

/* allocate memory for creating loop */
void LoopGen::initArray() {
    LoopCell ** loop = new LoopCell*[m_];
    for (int i = 0; i < m_; i++) {
        loop_[i] = new LoopCell[n_];
    }
}

/* reallocate memory */
void LoopGen::destroyArray() {
    for (int i = 0; i < m_; i++) {
        delete [] loop_[i];
    }
    delete [] loop_;
}

/* Fill grid entirely with numbers that make a loop */
void LoopGen::genLoop() {
    Coordinates pos = { m_/2, n_/2 };
    bool condition = false;

    for (int cat = 0; cat < 10; cat++) {
        if (getLoopCell(pos.i - 1, pos.j) == UNKNOWN ||
            getLoopCell(pos.i + 1, pos.j) == UNKNOWN ||
            getLoopCell(pos.i, pos.j - 1) == UNKNOWN ||
            getLoopCell(pos.i, pos.j + 1) == UNKNOWN) {
            loop_[pos.i][pos.j] = IN;
        } else {
            loop_[pos.i][pos.j] = OUT;
        }

    }
}

LoopCell LoopGen::getLoopCell(int i, int j) const {
    if (i < 0 || i >= m_ || j < 0 || j >=n_) {
        return OUT;
    } else {
        return loop_[i][j];
    }
}

/* randomly select one of the adjacent cells that is
 * UNKNOWN and set position to those coordinates */
Coordinates LoopGen::chooseNext(int i, int j) const {
    srand(time(NULL));
    bool foundnext = false;

    while (!foundnext) {
        int direction = rand() % 4;

        switch (direction) {
            case 0:
                i--;
                break;
            case 1:
                i++;
                break;
            case 2:
                j++;
                break;
            case 3:
                j--;
                break;
        }

        if (getLoopCell(i, j) == UNKNOWN) {
            foundnext = true;
            return Coordinates { i, j };
        }
    }
}
