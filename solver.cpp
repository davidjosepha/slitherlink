#include "solver.h"
#include "enums.h"
#include "grid.h"
#include "rule.h"
#include "contradiction.h"
#include <stdio.h>

/* Constructor takes a grid as input to solve */
Solver::Solver(Grid & grid, int depth) {
    grid_ = &grid;
    depth_ = depth;
    initRules();
    initContradictions();

    solve();
}

void Solver::solve() {
    while (grid_->getUpdated() && !grid_->isSolved()) {
        while (grid_->getUpdated() && !grid_->isSolved()) {
            grid_->setUpdated(false);
            applyRules();
        }

        if (depth_ > 0 && !testContradictions()) {
            makeGuesses();
        }
    }
}

void Solver::makeGuesses() {
    for (int i = 0; i < grid_->getHeight()+1; i++) {
        for (int j = 0; j < grid_->getWidth(); j++) {
            if (grid_->getHLine(i, j) == EMPTY) {
                /* there is only one case where the grid
                 * will not be updated, which is handled
                 * at the end of this iteration. */
                grid_->setUpdated(true);

                Grid lineGuess;
                grid_->copy(lineGuess);

                lineGuess.setHLine(i, j, LINE);
                Solver lineSolver = Solver(lineGuess, depth_-1);

                if (lineGuess.isSolved()) {
                    lineGuess.copy(*grid_);
                    return;
                } else if (!lineGuess.getIsValid()) {
                    grid_->setHLine(i, j, NLINE);
                    return;
                } else {
                    Grid nLineGuess;
                    grid_->copy(nLineGuess);

                    nLineGuess.setHLine(i, j, NLINE);
                    Solver nLineSolver = Solver(nLineGuess, depth_-1);

                    if (nLineGuess.isSolved()) {
                        nLineGuess.copy(*grid_);
                        return;
                    } else if (!nLineGuess.getIsValid()) {
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
    }

}

/* Checks for the intersection between lineGuess and nLineGuess grids
 * and applies any intersection to the canonical grid. */
void Solver::intersectGrids(Grid const & lineGuess, Grid const & nLineGuess) {
    /* TODO: add assertion that m and n are the same for all grids */

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
 * applying it. */
void Solver::applyRules() {
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

/* Runs a loop testing each contradiction in each orientation in
 * each valid position on the grid, checking if the contradiction
 * applies, and, if so, returning true. */
bool Solver::testContradictions() {
    for (int x = 0; x < NUM_CONTS; x++) {
        for (Orientation orient: (Orientation[]){ UP, DOWN, LEFT, RIGHT, UPFLIP, DOWNFLIP, LEFTFLIP, RIGHTFLIP }) {
            for (int i = 0; i <= grid_->getHeight() - contradictions_[x].getNumberHeight(orient); i++) {
                for (int j = 0; j <= grid_->getWidth() - contradictions_[x].getNumberWidth(orient); j++) {
                    if (contradictionApplies(i, j, contradictions_[x], orient)) {
                        printf(" %i , m %i , n %i \n ", x, i, j);
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
                grid_->setIsValid(grid_->setHLine(k + i, l + j, rule.getHLineAfter(k, l, orient)));
                grid_->setUpdated(true);
            }
        }
    }

    for (int k = 0; k < rule.getVLineHeight(orient); k++) {
        for (int l = 0; l < rule.getVLineWidth(orient); l++) {
            if (rule.getVLineAfter(k, l, orient) != EMPTY && grid_->getVLine(k + i, l + j) == EMPTY) {
                grid_->setIsValid(grid_->setVLine(k + i, l + j, rule.getVLineAfter(k, l, orient)));
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

    grid_->setIsValid(false);
    return true;
}

/* Initializes the rules_ array with each deterministic rule
 * used by the Solver to complete the grid. By convention the
 * rules will be represented with width <= height, although
 * each rule will be applied in each possible orientation. */
void Solver::initRules() {
    int i = 0;

    /**
     * Rule #01
     * Before       After
     * .   .   .    .   .   .
     *     x
     * . x . x .    .   .   .
     *                  x
     * .   .   .    .   .   .
     */
    beforeLattices_[i].initArrays(2, 2);

    beforeLattices_[i].setHLine(1, 1, NLINE);
    beforeLattices_[i].setHLine(1, 0, NLINE);
    beforeLattices_[i].setVLine(0, 1, NLINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setVLine(1, 1, NLINE);


    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #02
     * Before       After
     * .   .   .    .   .   .
     *     |
     * . - .   .    .   . x .
     *                  x
     * .   .   .    .   .   .
     */
    beforeLattices_[i].initArrays(2, 2);

    beforeLattices_[i].setHLine(1, 0, LINE);
    beforeLattices_[i].setVLine(0, 1, LINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setVLine(1, 1, NLINE);
    afterLattices_[i].setHLine(1, 1, NLINE);


    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #03
     * Before       After
     * .   .   .    .   .   .
     *                  x
     * . - . - .    .   .   .
     *                  x
     * .   .   .    .   .   .
     */
    beforeLattices_[i].initArrays(2, 2);

    beforeLattices_[i].setHLine(1, 0, LINE);
    beforeLattices_[i].setHLine(1, 1, LINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setVLine(0, 1, NLINE);
    afterLattices_[i].setVLine(1, 1, NLINE);


    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #04
     * Before       After
     * .   .   .    .   .   .
     *     x
     * . - .   .    .   . - .
     *     x
     * .   .   .    .   .   .
     */
    beforeLattices_[i].initArrays(2, 2);

    beforeLattices_[i].setHLine(1, 0, LINE);
    beforeLattices_[i].setVLine(0, 1, NLINE);
    beforeLattices_[i].setVLine(1, 1, NLINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(1, 1, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #05
     * Before       After
     * .   .   .    .   .   .
     *     x
     * . - . x .    .   .   .
     *                  |
     * .   .   .    .   .   .
     */
    beforeLattices_[i].initArrays(2, 2);

    beforeLattices_[i].setHLine(1, 0, LINE);
    beforeLattices_[i].setHLine(1, 1, NLINE);
    beforeLattices_[i].setVLine(0, 1, NLINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setVLine(1, 1, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #06
     * Before   After
     * . - .    .   .
     *   1      x   x
     * .   .    . x .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, ONE);
    beforeLattices_[i].setHLine(0, 0, LINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(1, 0, NLINE);
    afterLattices_[i].setVLine(0, 0, NLINE);
    afterLattices_[i].setVLine(0, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #07
     * Before   After
     * .   .    . - .
     * x 1 x
     * . x .    .   .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, ONE);
    beforeLattices_[i].setHLine(1, 0, NLINE);
    beforeLattices_[i].setVLine(0, 0, NLINE);
    beforeLattices_[i].setVLine(0, 1, NLINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(0, 0, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #08
     * Before   After
     * . - .    .   .
     * | 2          x
     * .   .    . x .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, TWO);
    beforeLattices_[i].setHLine(0, 0, LINE);
    beforeLattices_[i].setVLine(0, 0, LINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(1, 0, NLINE);
    afterLattices_[i].setVLine(0, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #09
     * Before   After
     * .   .    . - .
     *   2 x    |
     * . x .    .   .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, TWO);
    beforeLattices_[i].setHLine(1, 0, NLINE);
    beforeLattices_[i].setVLine(0, 1, NLINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(0, 0, LINE);
    afterLattices_[i].setVLine(0, 0, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #10
     * Before   After
     * . - .    .   .
     *   2      x   x
     * . - .    .   .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, TWO);
    beforeLattices_[i].setHLine(0, 0, LINE);
    beforeLattices_[i].setHLine(1, 0, LINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setVLine(0, 0, NLINE);
    afterLattices_[i].setVLine(0, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #11
     * Before   After
     * .   .    . - .
     * x 2 x
     * .   .    . - .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, TWO);
    beforeLattices_[i].setVLine(0, 0, NLINE);
    beforeLattices_[i].setVLine(0, 1, NLINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(0, 0, LINE);
    afterLattices_[i].setHLine(1, 0, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #12
     * Before   After
     * .   .    . x .
     * | 3 |
     * . - .    .   .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, THREE);
    beforeLattices_[i].setVLine(0, 0, LINE);
    beforeLattices_[i].setVLine(0, 1, LINE);
    beforeLattices_[i].setHLine(1, 0, LINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(0, 0, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #13
     * Before   After
     * . x .    .   .
     *   3      |   |
     * .   .    . - .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, THREE);
    beforeLattices_[i].setHLine(0, 0, NLINE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setVLine(0, 0, LINE);
    afterLattices_[i].setVLine(0, 1, LINE);
    afterLattices_[i].setHLine(1, 0, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule #14
     * Before   After
     * .   .    . x .
     *   0      x   x
     * .   .    . x .
     */
    beforeLattices_[i].initArrays(1, 1);

    beforeLattices_[i].setNumber(0, 0, ZERO);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(0, 0, NLINE);
    afterLattices_[i].setHLine(1, 0, NLINE);
    afterLattices_[i].setVLine(0, 0, NLINE);
    afterLattices_[i].setVLine(0, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 15
     * Before         After
     * .   .   .   .      .   .   .   .
     *         |
     * .   .   . x .      .   .   .   .
     *       1                x
     * .   .   .   .      .   . x .   .
     *
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setVLine(0, 2, LINE);
    beforeLattices_[i].setHLine(1, 2, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setVLine(1, 1, NLINE);
    afterLattices_[i].setHLine(2, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 16
     * Before         After
     * .   .   .   .      .   .   .   .
     *         |
     * .   .   .   .      .   .   . x .
     *     x 1
     * .   . x .   .      .   .   .   .
     *
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setVLine(0, 2, LINE);
    beforeLattices_[i].setVLine(1, 1, NLINE);
    beforeLattices_[i].setHLine(2, 1, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setHLine(1, 2, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 17
     * Before         After
     * .   .   .   .      .   .   .   .
     *
     * .   .   .   .      .   .   .   .
     *       1                x
     * . x .   .   .      .   . x .   .
     *     x
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setVLine(2, 1, NLINE);
    beforeLattices_[i].setHLine(2, 0, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setVLine(1, 1, NLINE);
    afterLattices_[i].setHLine(2, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 18
     * Before         After
     * .   .   .   .      .   .   .   .
     *     |   |
     * . x .   . x .      .   . _ .   .
     *       1
     * .   .   .   .      .   .   .   .
     *
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setVLine(0, 1, LINE);
    beforeLattices_[i].setVLine(0, 2, LINE);
    beforeLattices_[i].setHLine(1, 2, NLINE);
    beforeLattices_[i].setHLine(1, 0, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setHLine(1, 1, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 19
     * Before         After
     * .   .   .   .      .   .   .   .
     *
     * .   . x .   .      .   .   .   .
     *       2                |
     * . x .   .   .      .   . _ .   .
     *     x
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, TWO);
    beforeLattices_[i].setVLine(2, 1, NLINE);
    beforeLattices_[i].setHLine(2, 0, NLINE);
    beforeLattices_[i].setHLine(1, 1, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setHLine(2, 1, LINE);
    afterLattices_[i].setVLine(1, 1, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 20
     * Before         After
     * .   .   .   .      .   .   .   .
     *     x   x
     * .   .   . x .      . _ .   .   .
     *       2
     * .   .   . x .      .   .   .   .
     *                            |
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, TWO);
    beforeLattices_[i].setVLine(0, 1, NLINE);
    beforeLattices_[i].setVLine(0, 2, NLINE);
    beforeLattices_[i].setHLine(1, 2, NLINE);
    beforeLattices_[i].setHLine(2, 2, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setHLine(1, 0, LINE);
    afterLattices_[i].setVLine(2, 2, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 21
     * Before         After
     * .   .   .   .      .   .   .   .
     *     x   x
     * .   .   . x .      . _ .   .   .
     *       2
     * .   .   .   .      .   .   .   .
     *
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, TWO);
    beforeLattices_[i].setVLine(0, 1, NLINE);
    beforeLattices_[i].setVLine(0, 2, NLINE);
    beforeLattices_[i].setHLine(1, 2, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setHLine(1, 0, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 22
     * Before         After
     * .   .   .   .      .   .   .   .
     *         |
     * .   .   .   .      .   .   . x .
     *       3                |
     * .   .   .   .      .   . _ .   .
     *
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, THREE);
    beforeLattices_[i].setVLine(0, 2, LINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setHLine(2, 1, LINE);
    afterLattices_[i].setHLine(1, 2, NLINE);
    afterLattices_[i].setVLine(1, 1, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 23
     * Before         After
     * .   .   .   .      .   .   .   .
     *
     * .   .   .   .      .   .   .   .
     *       3                |
     * . x .   .   .      .   . _ .   .
     *     x
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setNumber(1, 1, THREE);
    beforeLattices_[i].setVLine(2, 1, NLINE);
    beforeLattices_[i].setHLine(2, 0, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setHLine(2, 1, LINE);
    afterLattices_[i].setVLine(1, 1, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule 24
     * Before       After
     * .   .   .    .   . _ .
     *       3              |
     * .   .   .    .   .   .
     *   3          |
     * .   .   .    . _ .   .
     */
    beforeLattices_[i].initArrays(2, 2);

    beforeLattices_[i].setNumber(1, 0, THREE);
    beforeLattices_[i].setNumber(0, 1, THREE);

    afterLattices_[i].initArrays(2, 2);

    afterLattices_[i].setHLine(0, 1, LINE);
    afterLattices_[i].setHLine(2, 0, LINE);
    afterLattices_[i].setVLine(1, 0, LINE);
    afterLattices_[i].setVLine(0, 2, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /**
     * Rule 25
     * Before       After
     * .   .   .    .   .   .
     *                  x
     * .   .   .    .   .   .
     *   3   3      |   |   |
     * .   .   .    .   .   .
     *                  x
     * .   .   .    .   .   .
     */
    beforeLattices_[i].initArrays(3,2);

    beforeLattices_[i].setNumber(1, 0, THREE);
    beforeLattices_[i].setNumber(1, 1, THREE);

    afterLattices_[i].initArrays(3,2);

    afterLattices_[i].setVLine(1, 0, LINE);
    afterLattices_[i].setVLine(1, 1, LINE);
    afterLattices_[i].setVLine(1, 2, LINE);
    afterLattices_[i].setVLine(0, 1, NLINE);
    afterLattices_[i].setVLine(2, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 26
     * Before         After
     * .   .   .      .   .   .
     *   3   1
     * .   .   .      . _ .   .
     * x   x
     * .   .   .      .   .   .
     *
     */
    beforeLattices_[i].initArrays(2,2);

    beforeLattices_[i].setNumber(0, 0, THREE);
    beforeLattices_[i].setNumber(0, 1, ONE);
    beforeLattices_[i].setVLine(1, 0, NLINE);
    beforeLattices_[i].setVLine(1, 1, NLINE);

    afterLattices_[i].initArrays(2,2);

    afterLattices_[i].setHLine(1, 0, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 27
     * Before         After
     * .   .   .   .      .   .   .   .
     *     x   x
     * . x .   . _ .      .   .   .   .
     *
     * . x .   . x .      .   .   .   .
     *         x              |
     * .   .   .   .      .   .   .   .
     *
     */
    beforeLattices_[i].initArrays(3,3);

    beforeLattices_[i].setVLine(0, 1, NLINE);
    beforeLattices_[i].setVLine(0, 2, NLINE);
    beforeLattices_[i].setVLine(2, 2, NLINE);
    beforeLattices_[i].setHLine(1, 2, LINE);
    beforeLattices_[i].setHLine(1, 0, NLINE);
    beforeLattices_[i].setHLine(2, 0, NLINE);
    beforeLattices_[i].setHLine(2, 2, NLINE);

    afterLattices_[i].initArrays(3,3);

    afterLattices_[i].setVLine(2, 1, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 28
     * Before         After
     *
     * . _ .      .   .
     * |   |
     * .   .      . x .
     *
     */
    beforeLattices_[i].initArrays(1,1);

    beforeLattices_[i].setVLine(0, 0, LINE);
    beforeLattices_[i].setVLine(0, 1, LINE);
    beforeLattices_[i].setHLine(0, 0, LINE);

    afterLattices_[i].initArrays(1,1);

    afterLattices_[i].setHLine(1, 0, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 29
     * Before         After
     *
     * . x .   .     .   .   .
     * x 1
     * .   .   .     .   .   .
     *       1               x
     * .   .   .     .   . x .
     */
    beforeLattices_[i].initArrays(2,2);

    beforeLattices_[i].setNumber(0, 0, ONE);
    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setVLine(0, 0, NLINE);
    beforeLattices_[i].setHLine(0, 0, NLINE);

    afterLattices_[i].initArrays(2,2);

    afterLattices_[i].setHLine(2, 1, NLINE);
    afterLattices_[i].setVLine(1, 2, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 30
     * Before         After
     *
     * .   .   .     .   .   .
     *     x
     * . x .   .     .   . x .
     *       1           x
     * .   .   .     .   .   .
     */
    beforeLattices_[i].initArrays(2,2);

    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setVLine(0, 1, NLINE);
    beforeLattices_[i].setHLine(1, 0, NLINE);


    afterLattices_[i].initArrays(2,2);

    afterLattices_[i].setHLine(1, 1, NLINE);
    afterLattices_[i].setVLine(1, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 31
     * Before         After
     *
     * .   . x .     .   .   .
     *       2               |
     * . - .   .     .   .   .
     *                   x
     * .   .   .     .   .   .
     */
    beforeLattices_[i].initArrays(2,2);

    beforeLattices_[i].setNumber(0, 1, TWO);
    beforeLattices_[i].setHLine(0, 1, NLINE);
    beforeLattices_[i].setHLine(1, 0, LINE);


    afterLattices_[i].initArrays(2,2);

    afterLattices_[i].setVLine(0, 2, LINE);
    afterLattices_[i].setVLine(1, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 32
     * Before         After
     *
     * .   .   .     .   . - .
     *       2               |
     * . - .   .     .   .   .
     *     |
     * .   .   .     .   .   .
     */
    beforeLattices_[i].initArrays(2,2);

    beforeLattices_[i].setNumber(0, 1, TWO);
    beforeLattices_[i].setHLine(1, 0, LINE);
    beforeLattices_[i].setVLine(1, 1, LINE);


    afterLattices_[i].initArrays(2,2);

    afterLattices_[i].setHLine(0, 1, LINE);
    afterLattices_[i].setVLine(0, 2, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 33
     * Before         After
     *
     * . - .   .     .   .   .
     * | 3
     * .   .   .     .   .   .
     *       1               x
     * .   .   .     .   . x .
     */
    beforeLattices_[i].initArrays(2,2);

    beforeLattices_[i].setNumber(0, 0, THREE);
    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setHLine(0, 0, LINE);
    beforeLattices_[i].setVLine(0, 0, LINE);


    afterLattices_[i].initArrays(2,2);

    afterLattices_[i].setHLine(2, 1, NLINE);
    afterLattices_[i].setVLine(1, 2, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /** Rule 34
     * Before         After
     *
     * .   .   .     . - .   .
     *   3           |
     * .   .   .     .   .   .
     *       1 x
     * .   . x .     .   .   .
     */
    beforeLattices_[i].initArrays(2,2);

    beforeLattices_[i].setNumber(0, 0, THREE);
    beforeLattices_[i].setNumber(1, 1, ONE);
    beforeLattices_[i].setHLine(2, 1, NLINE);
    beforeLattices_[i].setVLine(1, 2, NLINE);

    afterLattices_[i].initArrays(2,2);

    afterLattices_[i].setHLine(0, 0, LINE);
    afterLattices_[i].setVLine(0, 0, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;
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
