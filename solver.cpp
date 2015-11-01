#include "solver.h"
#include <cassert>
#include "contradiction.h"
#include "enums.h"
#include "grid.h"
#include "rule.h"

/* Constructor takes a grid as input to solve */
Solver::Solver(Grid & grid, int depth) {
    grid_ = &grid;
    depth_ = depth;
    initRules();
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
        Solver lineSolver = Solver(lineGuess, depth);

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
            Solver nLineSolver = Solver(nLineGuess, depth);

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
        Solver lineSolver = Solver(lineGuess, depth);

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
            Solver nLineSolver = Solver(nLineGuess, depth);

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

/* Initializes the rules_ array with each deterministic rule
 * used by the Solver to complete the grid. By convention the
 * rules will be represented with width <= height, although
 * each rule will be applied in each possible orientation. */
void Solver::initRules() {
    int i = 0;
    Lattice * before;
    Lattice * after;

    /**
     * Rule #01
     * Before       After
     * .   .   .    .   .   .
     *     x
     * . x . x .    .   .   .
     *                  x
     * .   .   .    .   .   .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setHLine(1, 1, NLINE);
    before->setHLine(1, 0, NLINE);
    before->setVLine(0, 1, NLINE);

    after->setVLine(1, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setHLine(1, 0, LINE);
    before->setVLine(0, 1, LINE);

    after->setVLine(1, 1, NLINE);
    after->setHLine(1, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setHLine(1, 0, LINE);
    before->setHLine(1, 1, LINE);

    after->setVLine(0, 1, NLINE);
    after->setVLine(1, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setHLine(1, 0, LINE);
    before->setVLine(0, 1, NLINE);
    before->setVLine(1, 1, NLINE);

    after->setHLine(1, 1, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setHLine(1, 0, LINE);
    before->setHLine(1, 1, NLINE);
    before->setVLine(0, 1, NLINE);

    after->setVLine(1, 1, LINE);

    i++;

    /**
     * Rule #06
     * Before   After
     * . - .    .   .
     *   1      x   x
     * .   .    . x .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, ONE);
    before->setHLine(0, 0, LINE);

    after->setHLine(1, 0, NLINE);
    after->setVLine(0, 0, NLINE);
    after->setVLine(0, 1, NLINE);

    i++;

    /**
     * Rule #07
     * Before   After
     * .   .    . - .
     * x 1 x
     * . x .    .   .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, ONE);
    before->setHLine(1, 0, NLINE);
    before->setVLine(0, 0, NLINE);
    before->setVLine(0, 1, NLINE);

    after->setHLine(0, 0, LINE);

    i++;

    /**
     * Rule #08
     * Before   After
     * . - .    .   .
     * | 2          x
     * .   .    . x .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, TWO);
    before->setHLine(0, 0, LINE);
    before->setVLine(0, 0, LINE);

    after->setHLine(1, 0, NLINE);
    after->setVLine(0, 1, NLINE);

    i++;

    /**
     * Rule #09
     * Before   After
     * .   .    . - .
     *   2 x    |
     * . x .    .   .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, TWO);
    before->setHLine(1, 0, NLINE);
    before->setVLine(0, 1, NLINE);

    after->setHLine(0, 0, LINE);
    after->setVLine(0, 0, LINE);

    i++;

    /**
     * Rule #10
     * Before   After
     * . - .    .   .
     *   2      x   x
     * . - .    .   .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, TWO);
    before->setHLine(0, 0, LINE);
    before->setHLine(1, 0, LINE);

    after->setVLine(0, 0, NLINE);
    after->setVLine(0, 1, NLINE);

    i++;

    /**
     * Rule #11
     * Before   After
     * .   .    . - .
     * x 2 x
     * .   .    . - .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, TWO);
    before->setVLine(0, 0, NLINE);
    before->setVLine(0, 1, NLINE);

    after->setHLine(0, 0, LINE);
    after->setHLine(1, 0, LINE);

    i++;

    /**
     * Rule #12
     * Before   After
     * .   .    . x .
     * | 3 |
     * . - .    .   .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, THREE);
    before->setVLine(0, 0, LINE);
    before->setVLine(0, 1, LINE);
    before->setHLine(1, 0, LINE);

    after->setHLine(0, 0, NLINE);

    i++;

    /**
     * Rule #13
     * Before   After
     * . x .    .   .
     *   3      |   |
     * .   .    . - .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, THREE);
    before->setHLine(0, 0, NLINE);

    after->setVLine(0, 0, LINE);
    after->setVLine(0, 1, LINE);
    after->setHLine(1, 0, LINE);

    i++;

    /**
     * Rule #14
     * Before   After
     * .   .    . x .
     *   0      x   x
     * .   .    . x .
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, ZERO);

    after->setHLine(0, 0, NLINE);
    after->setHLine(1, 0, NLINE);
    after->setVLine(0, 0, NLINE);
    after->setVLine(0, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, ONE);
    before->setVLine(0, 2, LINE);
    before->setHLine(1, 2, NLINE);

    after->setVLine(1, 1, NLINE);
    after->setHLine(2, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, ONE);
    before->setVLine(0, 2, LINE);
    before->setVLine(1, 1, NLINE);
    before->setHLine(2, 1, NLINE);

    after->setHLine(1, 2, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, ONE);
    before->setVLine(2, 1, NLINE);
    before->setHLine(2, 0, NLINE);

    after->setVLine(1, 1, NLINE);
    after->setHLine(2, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, ONE);
    before->setVLine(0, 1, LINE);
    before->setVLine(0, 2, LINE);
    before->setHLine(1, 2, NLINE);
    before->setHLine(1, 0, NLINE);

    after->setHLine(1, 1, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, TWO);
    before->setVLine(2, 1, NLINE);
    before->setHLine(2, 0, NLINE);
    before->setHLine(1, 1, NLINE);

    after->setHLine(2, 1, LINE);
    after->setVLine(1, 1, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, TWO);
    before->setVLine(0, 1, NLINE);
    before->setVLine(0, 2, NLINE);
    before->setHLine(1, 2, NLINE);
    before->setHLine(2, 2, NLINE);

    after->setHLine(1, 0, LINE);
    after->setVLine(2, 2, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, TWO);
    before->setVLine(0, 1, NLINE);
    before->setVLine(0, 2, NLINE);
    before->setHLine(1, 2, NLINE);

    after->setHLine(1, 0, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, THREE);
    before->setVLine(0, 2, LINE);

    after->setHLine(2, 1, LINE);
    after->setHLine(1, 2, NLINE);
    after->setVLine(1, 1, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, THREE);
    before->setVLine(2, 1, NLINE);
    before->setHLine(2, 0, NLINE);

    after->setHLine(2, 1, LINE);
    after->setVLine(1, 1, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 0, THREE);
    before->setNumber(0, 1, THREE);

    after->setHLine(0, 1, LINE);
    after->setHLine(2, 0, LINE);
    after->setVLine(1, 0, LINE);
    after->setVLine(0, 2, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 0, THREE);
    before->setNumber(1, 1, THREE);

    after->setVLine(1, 0, LINE);
    after->setVLine(1, 1, LINE);
    after->setVLine(1, 2, LINE);
    after->setVLine(0, 1, NLINE);
    after->setVLine(2, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, THREE);
    before->setNumber(0, 1, ONE);
    before->setVLine(1, 0, NLINE);
    before->setVLine(1, 1, NLINE);

    after->setHLine(1, 0, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(3, 3);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setVLine(0, 1, NLINE);
    before->setVLine(0, 2, NLINE);
    before->setVLine(2, 2, NLINE);
    before->setHLine(1, 2, LINE);
    before->setHLine(1, 0, NLINE);
    before->setHLine(2, 0, NLINE);
    before->setHLine(2, 2, NLINE);

    after->setVLine(2, 1, LINE);

    i++;

    /** Rule 28
     * Before         After
     *
     * . _ .      .   .
     * |   |
     * .   .      . x .
     *
     */

    rules_[i] = Rule();
    rules_[i].initLattices(1, 1);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setVLine(0, 0, LINE);
    before->setVLine(0, 1, LINE);
    before->setHLine(0, 0, LINE);

    after->setHLine(1, 0, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, ONE);
    before->setNumber(1, 1, ONE);
    before->setVLine(0, 0, NLINE);
    before->setHLine(0, 0, NLINE);

    after->setHLine(2, 1, NLINE);
    after->setVLine(1, 2, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(1, 1, ONE);
    before->setVLine(0, 1, NLINE);
    before->setHLine(1, 0, NLINE);


    after->setHLine(1, 1, NLINE);
    after->setVLine(1, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 1, TWO);
    before->setHLine(0, 1, NLINE);
    before->setHLine(1, 0, LINE);


    after->setVLine(0, 2, LINE);
    after->setVLine(1, 1, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 1, TWO);
    before->setHLine(1, 0, LINE);
    before->setVLine(1, 1, LINE);

    after->setHLine(0, 1, LINE);
    after->setVLine(0, 2, LINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, THREE);
    before->setNumber(1, 1, ONE);
    before->setHLine(0, 0, LINE);
    before->setVLine(0, 0, LINE);

    after->setHLine(2, 1, NLINE);
    after->setVLine(1, 2, NLINE);

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

    rules_[i] = Rule();
    rules_[i].initLattices(2, 2);

    before = rules_[i].getBefore();
    after = rules_[i].getAfter();

    before->setNumber(0, 0, THREE);
    before->setNumber(1, 1, ONE);
    before->setHLine(2, 1, NLINE);
    before->setVLine(1, 2, NLINE);

    after->setHLine(0, 0, LINE);
    after->setVLine(0, 0, LINE);

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
