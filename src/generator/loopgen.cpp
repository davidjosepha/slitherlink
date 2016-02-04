#include "loopgen.h"
#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "../solver/enums.h"
#include "../solver/grid.h"
#include "../solver/structs.h"

/* LoopGen constructor */
LoopGen::LoopGen(int m, int n, Grid & grid) {
    m_ = m;
    n_ = n;
    grid_ = &grid;
    srand(time(NULL));

    initArray();
    genLoop();
    fillGrid();
    destroyArray();
}

void LoopGen::fillGrid() {
    /*
    for (int i = 0; i < m_; i++) {
        for (int j = 0; j < n_; j++) {
            if (loop_[i][j] != UNKNOWN && loop_[i][j] != OUT) {
                //printf("%d", loop_[i][j]);
                printf("X");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    */

    int lines;
    for (int i = 1; i < m_ + 1; i++) {
        for (int j = 1; j < n_ + 1; j++) {
            lines = countLines(i - 1, j - 1);

            switch (lines) {
                case 0:
                    grid_->setNumber(i, j, ZERO);
                    break;
                case 1:
                    grid_->setNumber(i, j, ONE);
                    break;
                case 2:
                    grid_->setNumber(i, j, TWO);
                    break;
                case 3:
                    grid_->setNumber(i, j, THREE);
                    break;
            }
        }
    }
}

int LoopGen::countLines(int i, int j) const {
    int lines = 0;

    bool inside = inLoop(i, j);

    if ((!inBounds({ i + 1, j}) && inside) || (inBounds({ i + 1, j }) && inLoop(i + 1, j) != inside)) {
        lines++;
    }

    if ((!inBounds({ i - 1, j}) && inside) || (inBounds({ i - 1, j }) && inLoop(i - 1, j) != inside)) {
        lines++;
    }

    if ((!inBounds({ i, j + 1}) && inside) || (inBounds({ i, j + 1 }) && inLoop(i, j + 1) != inside)) {
        lines++;
    }

    if ((!inBounds({ i, j - 1}) && inside) || (inBounds({ i, j - 1 }) && inLoop(i, j - 1) != inside)) {
        lines++;
    }

    return lines;
}

bool LoopGen::inLoop(int i, int j) const {
    return loop_[i][j] == EXP || loop_[i][j] == NOEXP;
}

/* allocate memory for creating loop */
void LoopGen::initArray() {
    loop_ = new LoopCell*[m_];
    for (int i = 0; i < m_; i++) {
        loop_[i] = new LoopCell[n_];

        for (int j = 0; j < n_; j++) {
            loop_[i][j] = UNKNOWN;
        }
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
    Coordinates cur = { m_ / 2, n_ / 2 };
    Coordinates next;
    std::vector<Coordinates> avail;
    avail.push_back(cur);

    for (int i = 0; i < 300000; i++) {
        //printf("%d\n", avail.size());
        cur = pickCell(avail);

        if (cur.i == -1 || cur.j == -1) {
            printf("Oh?");
            return;
        }

        next = addCell(cur);

        if (loop_[cur.i][cur.j] == EXP) {
            addAvailable(cur, avail);
        }
        if (loop_[next.i][next.j] == EXP) {
            addAvailable(next, avail);
        }
    }
}

Coordinates LoopGen::addCell(Coordinates cur) {
    assert(cur.i >= 0 && cur.i < m_ && cur.j >= 0 && cur.j < n_);

    /* check whether it's possible to expand in any direction */
    if (!isExpandable(cur)) {
        return cur;
    }

    /* pick some direction up/down/left/right from cur */
    Coordinates newpos = pickDirection(cur);

    /* and verify it's a valid choice */
    if (!inBounds(newpos) || loop_[newpos.i][newpos.j] != UNKNOWN) {
        return cur;
    }

    /*
    std::vector<Coordinates> directions = std::vector<Coordinates>();
    pickDirections(directions, cur);
    Coordinates newpos;

    for (int i = 0; i < directions.size(); i++) {
        newpos = directions[i];
        if (inBounds(newpos) && loop_[newpos.i][newpos.j] == UNKNOWN) {
            break;
        }
        return cur;
    }
    */

    AdjacencyList adjacencyList = getAdjacent(newpos);

    if (!adjacencyList.u && !adjacencyList.d && !adjacencyList.l && !adjacencyList.r) {
        loop_[newpos.i][newpos.j] = OUT;
        return newpos;
    }

    loop_[newpos.i][newpos.j] = (validCell(newpos, cur)) ? EXP : OUT;
    return newpos;
}

/*
void LoopGen::pickDirections(std::vector<Coordinates> & directions, Coordinates cur) const {
    directions.push_back({ cur.i + 1, cur.j });
    directions.push_back({ cur.i - 1, cur.j });
    directions.push_back({ cur.i, cur.j + 1 });
    directions.push_back({ cur.i, cur.j - 1 });

    std::random_shuffle(directions.begin(), directions.end());
}
*/

/* pick some direction up/down/left/right from cur */
Coordinates LoopGen::pickDirection(Coordinates cur) const {
    int vert = (rand() % 3) - 1;
    int hor = 0;
    if (vert == 0) {
        hor = (rand() % 2)*2 - 1;
    }
    return { cur.i + vert, cur.j + hor };
}

/* check whether it's possible to expand in at least one direction */
bool LoopGen::isExpandable(Coordinates cur) {
    assert(cur.i >= 0 && cur.i < m_ && cur.j >= 0 && cur.j < n_);

    AdjacencyList adjacencyList = getAdjacent(cur);

    if (!adjacencyList.u && !adjacencyList.d && !adjacencyList.l && !adjacencyList.r) {
        loop_[cur.i][cur.j] = NOEXP;
        return false;
    }

    return true;
}

/* Return which cells relative to the current cell are available */
AdjacencyList LoopGen::getAdjacent(Coordinates cur) const {
    AdjacencyList adjacencyList = { .u = validCell({ cur.i - 1, cur.j }, cur),
                                    .d = validCell({ cur.i + 1, cur.j }, cur),
                                    .l = validCell({ cur.i, cur.j - 1 }, cur),
                                    .r = validCell({ cur.i, cur.j + 1 }, cur) };
    return adjacencyList;
}

void LoopGen::addAvailable(Coordinates coords, std::vector<Coordinates> & avail) const {
    for (int i = 0; i < avail.size(); i++) {
        if (avail[i].i == coords.i && avail[i].j == coords.j) {
            return;
        }
    }

    avail.push_back(coords);
}

Coordinates LoopGen::pickCell(std::vector<Coordinates> & avail) const {
    Coordinates guess;

    if (avail.size() > 0) {
        guess = avail.back();
        int guessindex = rand() % avail.size();
        guess = avail[guessindex];

        avail.erase(avail.begin() + guessindex);
        return guess;
    } else {
        return { -1, -1 };
    }
}

/* Don't change this function, David. Like really, don't. You're gonna fuck something up like you always do. */
bool LoopGen::validCell(Coordinates coords, Coordinates cur) const {
    bool valid = true;

    /* check to make sure the cell is within the grid */
    if (!inBounds(coords)) {
        return false;
    }

    valid = valid && loop_[coords.i][coords.j] != NOEXP;
    valid = valid && loop_[coords.i][coords.j] != OUT;

    Coordinates shift = { coords.i - cur.i, coords.j - cur.j };

    if ((shift.i + shift.j != 1 && shift.i + shift.j != -1) || shift.i * shift.j != 0) {
        valid = false;
    }

    if (shift.i == 0) {
        valid = valid && cellOpen(coords.i, coords.j + shift.j);

        if (coords.i == 0) {
            valid = valid && cellOpen(1, coords.j + shift.j);
        } else if (coords.i == m_ - 1) {
            valid = valid && cellOpen(m_ - 2, coords.j + shift.j);
        }
        valid = valid && cellOpen(coords.i + 1, coords.j + shift.j);
        valid = valid && cellOpen(coords.i - 1, coords.j + shift.j);
    }

    if (shift.j == 0) {
        valid = valid && cellOpen(coords.i + shift.i, coords.j);

        if (coords.j == 0) {
            valid = valid && cellOpen(coords.i + shift.i, 1);
        } else if (coords.j == n_ - 1) {
            valid = valid && cellOpen(coords.i + shift.i, n_ - 2);
        }
        valid = valid && cellOpen(coords.i + shift.i, coords.j + 1);
        valid = valid && cellOpen(coords.i + shift.i, coords.j - 1);
    }

    if (!valid && loop_[coords.i][coords.j] == UNKNOWN) {
        loop_[coords.i][coords.j] = OUT;
    }

    return valid;
}

bool LoopGen::cellOpen(int i, int j) const {
    Coordinates coords = { i, j };
    return !inBounds(coords) || loop_[coords.i][coords.j] == UNKNOWN || loop_[coords.i][coords.j] == OUT;
}

bool LoopGen::inBounds(Coordinates coords) const {
    return coords.i >= 0 && coords.j >= 0 && coords.i < m_ && coords.j < n_;
}
