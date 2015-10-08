#include "solver.h"
#include "enums.h"
#include "lattice.h"
#include "rule.h"

/* Constructor takes a grid as input to solve */
Solver::Solver(Lattice & grid) {
    grid_ = &grid;
    initRules();

    while (grid_->getUpdated()) {
        grid_->setUpdated(false);
        applyRules();
    }
}

/* Runs a loop checking each rule in each orientation in each valid
 * position on the grid, checking if the rule applies, and, if so,
 * applying it. */
void Solver::applyRules() {
    for  (int x = 0; x < NUM_RULES; x++) {
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
                grid_->setHLine(k + i, l + j, rule.getHLineAfter(k, l, orient));
                grid_->setUpdated(true);
            }
        }
    }

    for (int k = 0; k < rule.getVLineHeight(orient); k++) {
        for (int l = 0; l < rule.getVLineWidth(orient); l++) {
            if (rule.getVLineAfter(k, l, orient) != EMPTY && grid_->getVLine(k + i, l + j) == EMPTY) {
                grid_->setVLine(k + i, l + j, rule.getVLineAfter(k, l, orient));
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
            if (rule.getNumberBefore(k, l, orient) != NONE and
                rule.getNumberBefore(k, l, orient) != grid_->getNumber(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < rule.getHLineHeight(orient); k++) {
        for (int l = 0; l < rule.getHLineWidth(orient); l++) {
            if (rule.getHLineBefore(k, l, orient) != EMPTY and
                rule.getHLineBefore(k, l, orient) != grid_->getHLine(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < rule.getVLineHeight(orient); k++) {
        for (int l = 0; l < rule.getVLineWidth(orient); l++) {
            if (rule.getVLineBefore(k, l, orient) != EMPTY and
                rule.getVLineBefore(k, l, orient) != grid_->getVLine(k + i, l + j)) {
                return false;
            }
        }
    }
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
    
    /**
     * Rule 19
     * Before       After
     * .   .   .    .   . - .
     *       3              |
     * .   .   .    .   .   .
     *   3          |
     * .   .   .    . - .   .
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
     * Rule 20
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

    /** Rule 21
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
    
    /** Rule 22
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
}
