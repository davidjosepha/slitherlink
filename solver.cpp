#include "solver.h"
#include <cassert>
#include "constants.h"
#include "contradiction.h"
#include "enums.h"
#include "grid.h"
#include "rule.h"
#include "contradiction.h"
#include <stdio.h>
#include <stdlib.h>

/* Constructor takes a grid as input to solve */
Solver::Solver(Grid & grid, Rule rules[NUM_RULES], int depth) {
    grid_ = &grid;
    depth_ = depth;
    int * prevm, * prevn;
    int x = 4;
    int y = 4;
    prevm = &x;
    prevn = &y;
    
    int done = 0;
    while (done == 0) {
        bool result = spiralNext(4, 4, prevm, prevn);
        if (!result) {
            done = 1;
        }
    }
    rules_ = rules;

    initContradictions();

    solve();
}

/* Apply a combination of deterministic rules and
 * recursive guessing to find a solution to a puzzle */
void Solver::solve() {
    while (grid_->getUpdated() && !grid_->isSolved()) {
        applyRules();

        for (int d = 0; d < depth_; d++) {
            if (!grid_->getUpdated() && !testContradictions() && !grid_->isSolved()) {
                solveDepth(d);
            }
        }
    }
}

/* Make a guess in each valid position in the graph */
void Solver::solveDepth(int depth) {
    for (int i = 0; i < grid_->getHeight()+1; i++) {
        for (int j = 0; j < grid_->getWidth(); j++) {
            applyRules();
            makeHLineGuess(i, j, depth);
        }
    }

    for (int i = 0; i < grid_->getHeight(); i++) {
        for (int j = 0; j < grid_->getWidth()+1; j++) {
            applyRules();
            makeVLineGuess(i, j, depth);
        }
    }
}

void Solver::makeHLineGuess(int i, int j, int depth) {
    assert(0 <= i && i < grid_->getHeight()+1 && 0 <= j && j < grid_->getWidth());
    assert(depth >= 0);

    if (grid_->getHLine(i, j) == EMPTY) {
        /* there is only one case where the grid
         * will not be updated, which is handled
         * at the end of this iteration. */
        grid_->setUpdated(true);

        Grid lineGuess;
        grid_->copy(lineGuess);

        lineGuess.setHLine(i, j, LINE);
        Solver lineSolver = Solver(lineGuess, rules_, depth);

        if (lineGuess.isSolved()) {
            lineGuess.copy(*grid_);
            return;
        } else if (!lineGuess.getValid()) {
            grid_->setHLine(i, j, NLINE);
            return;
        } else {
            Grid nLineGuess;
            grid_->copy(nLineGuess);

            nLineGuess.setHLine(i, j, NLINE);
            Solver nLineSolver = Solver(nLineGuess, rules_, depth);

            if (nLineGuess.isSolved()) {
                nLineGuess.copy(*grid_);
                return;
            } else if (!nLineGuess.getValid()) {
                grid_->setHLine(i, j, LINE);
                return;
            } else {
                grid_->setUpdated(false);
                intersectGrids(lineGuess, nLineGuess);

                if (grid_->getUpdated()) {
                    return;
                }
            }
        }
    }
}

void Solver::makeVLineGuess(int i, int j, int depth) {
    assert(0 <= i && i < grid_->getHeight() && 0 <= j && j < grid_->getWidth()+1);
    assert(depth >= 0);

    if (grid_->getVLine(i, j) == EMPTY) {
        /* there is only one case where the grid
         * will not be updated, which is handled
         * at the end of this iteration. */
        grid_->setUpdated(true);

        Grid lineGuess;
        grid_->copy(lineGuess);

        lineGuess.setVLine(i, j, LINE);
        Solver lineSolver = Solver(lineGuess, rules_, depth);

        if (lineGuess.isSolved()) {
            lineGuess.copy(*grid_);
            //free(lineGuess.contours_);
            return;
        } else if (!lineGuess.getValid()) {
            grid_->setVLine(i, j, NLINE);
            return;
        } else {
            Grid nLineGuess;
            grid_->copy(nLineGuess);

            nLineGuess.setVLine(i, j, NLINE);
            Solver nLineSolver = Solver(nLineGuess, rules_, depth);

            if (nLineGuess.isSolved()) {
                nLineGuess.copy(*grid_);
                return;
            } else if (!nLineGuess.getValid()) {
                grid_->setVLine(i, j, LINE);
                return;
            } else {
                grid_->setUpdated(false);
                intersectGrids(lineGuess, nLineGuess);

                if (grid_->getUpdated()) {
                    return;
                }
            }
        }
    }
}

bool Solver::spiralNext(int startm, int startn, int *prevm, int *prevn) {
    int tempm = *prevm;
    int tempn = *prevn;
    if (*prevm+*prevn == startm+startn) {
        if (*prevn >= startn) {
            (*prevn)++;
        } else {
            (*prevm)--;
        }
    } else if ((startm - *prevm) == (startn - *prevn)) {
        if (*prevm > startm) {
            (*prevn)--;
        } else {
            (*prevn)++;
        }
    } else if (*prevm > *prevn) {
        if (*prevm + *prevn > startm + startn) {
            if (*prevm > startm) {
                (*prevn)--;
            }
        } else {
            if (*prevn < startn) {
                (*prevm)--;
            }
        }
    } else {
        if (*prevm + *prevn > startm + startn) {
            if (*prevn > startn) {
                (*prevm)++;
            }
        } else {
            if (*prevm < startm) {
                (*prevn)++;
            }
        }
    }
    if ((tempm == *prevm && tempn == *prevn) || *prevm < 0 || *prevn < 0 || *prevm >= grid_->getHeight() || *prevn >= grid_->getWidth()) {
        return 0;
    } else {
        return 1;
    }
}


/* Checks for the intersection between lineGuess and nLineGuess grids
 * and applies any intersection to the canonical grid. */
void Solver::intersectGrids(Grid const & lineGuess, Grid const & nLineGuess) {
    assert(lineGuess.getHeight() == nLineGuess.getHeight() &&
           lineGuess.getWidth() == nLineGuess.getWidth());

    for (int i = 0; i < grid_->getHeight()+1; i++) {
        for (int j = 0; j < grid_->getWidth(); j++) {
            if (lineGuess.getHLine(i, j) == nLineGuess.getHLine(i, j) &&
                  lineGuess.getHLine(i, j) != grid_->getHLine(i, j)) {
                grid_->setHLine(i, j, lineGuess.getHLine(i, j));
                grid_->setUpdated(true);
            }
        }
    }

    for (int i = 0; i < grid_->getHeight(); i++) {
        for (int j = 0; j < grid_->getWidth()+1; j++) {
            if (lineGuess.getVLine(i, j) == nLineGuess.getVLine(i, j) &&
                  lineGuess.getVLine(i, j) != grid_->getVLine(i, j)) {
                grid_->setVLine(i, j, lineGuess.getVLine(i, j));
                grid_->setUpdated(true);
            }
        }
    }
}

/* Runs a loop checking each rule in each orientation in each valid
 * position on the grid, checking if the rule applies, and, if so,
 * applying it, and continue updating them until there are no longer
 * any changes being made. */
void Solver::applyRules() {
    while (grid_->getUpdated()) {
        grid_->setUpdated(false);

        for (int x = 0; x < NUM_RULES; x++) {
            for (Orientation orient : (Orientation[]){ UP, DOWN, LEFT, RIGHT, UPFLIP, DOWNFLIP, LEFTFLIP, RIGHTFLIP }) {
                for (int i = 0; i <= grid_->getHeight() - rules_[x].getNumberHeight(orient); i++) {
                    for (int j = 0; j <= grid_->getWidth() - rules_[x].getNumberWidth(orient); j++) {
                        if (ruleApplies(i, j, rules_[x], orient)) {
                            applyRule(i, j, rules_[x], orient);
                        }
                    }
                }
            }
        }
    }
}

/* Runs a loop testing each contradiction in each orientation in
 * each valid position on the grid, checking if the contradiction
 * applies, and, if so, returning true. */
bool Solver::testContradictions() {
    for (int x = 0; x < NUM_CONTS; x++) {
        for (Orientation orient: (Orientation[]){ UP, DOWN, LEFT, RIGHT, UPFLIP, DOWNFLIP, LEFTFLIP, RIGHTFLIP }) {
            for (int i = 0; i <= grid_->getHeight() - contradictions_[x].getNumberHeight(orient); i++) {
                for (int j = 0; j <= grid_->getWidth() - contradictions_[x].getNumberWidth(orient); j++) {
                    if (contradictionApplies(i, j, contradictions_[x], orient)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


/* Applies a rule in a given orientation to a given region of the
 * grid, overwriting all old values with any applicable values from
 * the after_ lattice for that rule. */
void Solver::applyRule(int i, int j, Rule & rule, Orientation orient) {
    for (int k = 0; k < rule.getNumberHeight(orient); k++) {
        for (int l = 0; l < rule.getNumberWidth(orient); l++) {
            if (rule.getNumberAfter(k, l, orient) != NONE && grid_->getNumber(k + i, l + j) == NONE) {
                grid_->setNumber(k + i, l + j, rule.getNumberAfter(k, l, orient));
                grid_->setUpdated(true);
            }
        }
    }

    for (int k = 0; k < rule.getHLineHeight(orient); k++) {
        for (int l = 0; l < rule.getHLineWidth(orient); l++) {
            if (rule.getHLineAfter(k, l, orient) != EMPTY && grid_->getHLine(k + i, l + j) == EMPTY) {
                grid_->setValid(grid_->setHLine(k + i, l + j, rule.getHLineAfter(k, l, orient)));
                grid_->setUpdated(true);
            }
        }
    }

    for (int k = 0; k < rule.getVLineHeight(orient); k++) {
        for (int l = 0; l < rule.getVLineWidth(orient); l++) {
            if (rule.getVLineAfter(k, l, orient) != EMPTY && grid_->getVLine(k + i, l + j) == EMPTY) {
                grid_->setValid(grid_->setVLine(k + i, l + j, rule.getVLineAfter(k, l, orient)));
                grid_->setUpdated(true);
            }
        }
    }
}

/* Checks if a rule in a given orientation applies to a given
 * region of the grid by checking all non-empty values in the
 * before_ lattice and verifying they correspond to the values
 * in the grid. */
bool Solver::ruleApplies(int i, int j, Rule & rule, Orientation orient) {
    for (int k = 0; k < rule.getNumberHeight(orient); k++) {
        for (int l = 0; l < rule.getNumberWidth(orient); l++) {
            if (rule.getNumberBefore(k, l, orient) != NONE &&
                rule.getNumberBefore(k, l, orient) != grid_->getNumber(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < rule.getHLineHeight(orient); k++) {
        for (int l = 0; l < rule.getHLineWidth(orient); l++) {
            if (rule.getHLineBefore(k, l, orient) != EMPTY &&
                rule.getHLineBefore(k, l, orient) != grid_->getHLine(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < rule.getVLineHeight(orient); k++) {
        for (int l = 0; l < rule.getVLineWidth(orient); l++) {
            if (rule.getVLineBefore(k, l, orient) != EMPTY &&
                rule.getVLineBefore(k, l, orient) != grid_->getVLine(k + i, l + j)) {
                return false;
            }
        }
    }
    return true;
}

/* Checks if a contradiction in a given orientation applies to a given
 * region of the grid by checking all non-empty values in the
 * cont_ lattice and verifying they correspond to the values
 * in the grid. */
bool Solver::contradictionApplies(int i, int j, Contradiction & contradiction, Orientation orient) {
    for (int k = 0; k < contradiction.getNumberHeight(orient); k++) {
        for (int l = 0; l < contradiction.getNumberWidth(orient); l++) {
            if (contradiction.getNumber(k, l, orient) != NONE &&
                contradiction.getNumber(k, l, orient) != grid_->getNumber(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < contradiction.getHLineHeight(orient); k++) {
        for (int l = 0; l < contradiction.getHLineWidth(orient); l++) {
            if (contradiction.getHLine(k, l, orient) != EMPTY &&
                contradiction.getHLine(k, l, orient) != grid_->getHLine(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < contradiction.getVLineHeight(orient); k++) {
        for (int l = 0; l < contradiction.getVLineWidth(orient); l++) {
            if (contradiction.getVLine(k, l, orient) != EMPTY &&
                contradiction.getVLine(k, l, orient) != grid_->getVLine(k + i, l + j)) {
                return false;
            }
        }
    }

    grid_->setValid(false);
    return true;
}

/* Initializes the contradictions_ array with each contradiction
 * used by the Solver to check the grid. By convention the
 * contradiction will be represented with width <= height, although
 * each contradiction will be checked in each possible orientation. */
void Solver::initContradictions() {
    int i = 0;

    /**
     * Contradiction #01
     * .   .   .
     *     x
     * . x . x .
     *     |
     * .   .   .
     */
    contLattices_[i].initArrays(2, 2);

    contLattices_[i].setHLine(1, 0, NLINE);
    contLattices_[i].setHLine(1, 1, NLINE);
    contLattices_[i].setVLine(0, 1, NLINE);
    contLattices_[i].setVLine(1, 1, LINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #02
     * .   .   .
     *     |
     * . _ . _ .
     *
     */
    contLattices_[i].initArrays(1, 2);

    contLattices_[i].setHLine(1, 0, LINE);
    contLattices_[i].setHLine(1, 1, LINE);
    contLattices_[i].setVLine(0, 1, LINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #03
     * . _ .
     * |   |
     * . _ .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setHLine(0, 0, LINE);
    contLattices_[i].setVLine(0, 0, LINE);
    contLattices_[i].setHLine(1, 0, LINE);
    contLattices_[i].setVLine(0, 1, LINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #04
     * . x .
     * x 3
     * .   .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, THREE);
    contLattices_[i].setHLine(0, 0, NLINE);
    contLattices_[i].setVLine(0, 0, NLINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #05
     * .   .
     * x 3 x
     * .   .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, THREE);
    contLattices_[i].setVLine(0, 0, NLINE);
    contLattices_[i].setVLine(0, 1, NLINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #06
     * . _ .
     * | 2
     * . _ .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, TWO);
    contLattices_[i].setHLine(0, 0, LINE);
    contLattices_[i].setHLine(1, 0, LINE);
    contLattices_[i].setVLine(0, 0, LINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #07
     * . x .
     * x 2
     * . x .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, TWO);
    contLattices_[i].setHLine(0, 0, NLINE);
    contLattices_[i].setHLine(1, 0, NLINE);
    contLattices_[i].setVLine(0, 0, NLINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #08
     * . _ .
     * | 1
     * .   .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, ONE);
    contLattices_[i].setHLine(0, 0, LINE);
    contLattices_[i].setVLine(0, 0, LINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #09
     * .   .
     * | 1 |
     * .   .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, ONE);
    contLattices_[i].setVLine(0, 0, LINE);
    contLattices_[i].setVLine(0, 1, LINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #10
     * . x .
     * x 1 x
     * . x .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, ONE);
    contLattices_[i].setVLine(0, 0, NLINE);
    contLattices_[i].setVLine(0, 1, NLINE);
    contLattices_[i].setHLine(0, 0, NLINE);
    contLattices_[i].setHLine(1, 0, NLINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;

    /**
     * Contradiction #11
     * .   .
     * | 0
     * .   .
     */
    contLattices_[i].initArrays(1, 1);

    contLattices_[i].setNumber(0, 0, ZERO);
    contLattices_[i].setVLine(0, 0, LINE);

    contradictions_[i] = Contradiction(contLattices_[i]);
    i++;
}
