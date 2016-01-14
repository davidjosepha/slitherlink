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

/* Fill grid entirely with numbers that make a loop */
void LoopGen::genLoop() {
    /* allocate memory for creating loop */
    LoopCell ** loop = new LoopCell*[m_];
    for (int i = 0; i < m_; i++) {
        loop[i] = new LoopCell[n_];
    }

    Coordinates position = { m_/2, n_/2 };
    bool condition = false;

    for (int cat = 0; cat < 10; cat++) {
        if (getLoopCell(position.i - 1, position.j, loop) == UNKNOWN ||
            getLoopCell(position.i + 1, position.j, loop) == UNKNOWN ||
            getLoopCell(position.i, position.j - 1, loop) == UNKNOWN ||
            getLoopCell(position.i, position.j + 1, loop) == UNKNOWN) {
            loop[position.i][position.j] = IN;
        } else {
            loop[position.i][position.j] = OUT;
        }

        /* randomly select one of the adjacent cells that is
         * UNKNOWN and set position to those coordinates */
        srand(time(NULL));
        bool foundnext = false;

        while (!foundnext) {
            int direction = rand() % 4;

            int i = position.i;
            int j = position.j;

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

            if (getLoopCell(i, j, loop) == UNKNOWN) {
                position.i = i;
                position.j = j;
                foundnext = true;
            }
        }
    }

    /* reallocate memory */
    for (int i = 0; i < m_; i++) {
        delete [] loop[i];
    }
    delete [] loop;
}

LoopCell LoopGen::getLoopCell(int i, int j, LoopCell ** loop) const {
    if (i < 0 || i >= m_ || j < 0 || j >=n_) {
        return OUT;
    } else {
        return loop[i][j];
    }
}
