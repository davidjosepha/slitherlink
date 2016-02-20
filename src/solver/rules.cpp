#include "rules.h"
#include "rule.h"
#include "../shared/constants.h"
#include "../shared/lattice.h"

/* Initializes the rules array with each deterministic rule
 * used by the Solver to complete the grid. By convention the
 * rules will be represented with width <= height, although
 * each rule will be applied in each possible orientation. */
void initRules(Rule (& rules)[NUM_RULES]) {
    int i = 0;
    Lattice * before;
    Lattice * after;

    /**
     * Rule #1
     * Before       After
     * .   .   .    .   .   .
     *     x
     * . x . x .    .   .   .
     *                  x
     * .   .   .    .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addHLinePattern(1, 0, NLINE);
    rules[i].addHLinePattern(1, 1, NLINE);
    rules[i].addVLinePattern(0, 1, NLINE);

    rules[i].addVLineDiff(1, 1, NLINE);

    i++;

    /**
     * Rule #2
     * Before       After
     * .   .   .    .   .   .
     *     |
     * . - .   .    .   . x .
     *                  x
     * .   .   .    .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addHLinePattern(1, 0, LINE);
    rules[i].addVLinePattern(0, 1, LINE);

    rules[i].addVLineDiff(1, 1, NLINE);
    rules[i].addHLineDiff(1, 1, NLINE);

    i++;


    /**
     * Rule #3
     * Before       After
     * .   .   .    .   .   .
     *                  x
     * . - . - .    .   .   .
     *                  x
     * .   .   .    .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addHLinePattern(1, 0, LINE);
    rules[i].addHLinePattern(1, 1, LINE);

    rules[i].addVLineDiff(0, 1, NLINE);
    rules[i].addVLineDiff(1, 1, NLINE);

    i++;

    /**
     * Rule #4
     * Before       After
     * .   .   .    .   .   .
     *     x
     * . - .   .    .   . - .
     *     x
     * .   .   .    .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addHLinePattern(1, 0, LINE);
    rules[i].addVLinePattern(0, 1, NLINE);
    rules[i].addVLinePattern(1, 1, NLINE);

    rules[i].addHLineDiff(1, 1, LINE);

    i++;

    /**
     * Rule #5
     * Before       After
     * .   .   .    .   .   .
     *     x
     * . - . x .    .   .   .
     *                  |
     * .   .   .    .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addHLinePattern(1, 0, LINE);
    rules[i].addHLinePattern(1, 1, NLINE);
    rules[i].addVLinePattern(0, 1, NLINE);

    rules[i].addVLineDiff(1, 1, LINE);

    i++;

    /**
     * Rule #6
     * Before   After
     * . - .    .   .
     *   1      x   x
     * .   .    . x .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, ONE);
    rules[i].addHLinePattern(0, 0, LINE);

    rules[i].addHLineDiff(1, 0, NLINE);
    rules[i].addVLineDiff(0, 0, NLINE);
    rules[i].addVLineDiff(0, 1, NLINE);

    i++;

    /**
     * Rule #7
     * Before   After
     * .   .    . - .
     * x 1 x
     * . x .    .   .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, ONE);
    rules[i].addHLinePattern(1, 0, NLINE);
    rules[i].addVLinePattern(0, 0, NLINE);
    rules[i].addVLinePattern(0, 1, NLINE);

    rules[i].addHLineDiff(0, 0, LINE);

    i++;

    /**
     * Rule #8
     * Before   After
     * . - .    .   .
     * | 2          x
     * .   .    . x .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, TWO);
    rules[i].addHLinePattern(0, 0, LINE);
    rules[i].addVLinePattern(0, 0, LINE);

    rules[i].addHLineDiff(1, 0, NLINE);
    rules[i].addVLineDiff(0, 1, NLINE);

    i++;

    /**
     * Rule #9
     * Before   After
     * .   .    . - .
     *   2 x    |
     * . x .    .   .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, TWO);
    rules[i].addHLinePattern(1, 0, NLINE);
    rules[i].addVLinePattern(0, 1, NLINE);

    rules[i].addHLineDiff(0, 0, LINE);
    rules[i].addVLineDiff(0, 0, LINE);

    i++;

    /**
     * Rule #10
     * Before   After
     * . - .    .   .
     *   2      x   x
     * . - .    .   .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, TWO);
    rules[i].addHLinePattern(0, 0, LINE);
    rules[i].addHLinePattern(1, 0, LINE);

    rules[i].addVLineDiff(0, 0, NLINE);
    rules[i].addVLineDiff(0, 1, NLINE);

    i++;

    /**
     * Rule #11
     * Before   After
     * .   .    . - .
     * x 2 x
     * .   .    . - .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, TWO);
    rules[i].addVLinePattern(0, 0, NLINE);
    rules[i].addVLinePattern(0, 1, NLINE);

    rules[i].addHLineDiff(0, 0, LINE);
    rules[i].addHLineDiff(1, 0, LINE);

    i++;

    /**
     * Rule #12
     * Before   After
     * . x .    .   .
     *   3      |   |
     * .   .    . - .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, THREE);
    rules[i].addHLinePattern(0, 0, NLINE);

    rules[i].addVLineDiff(0, 0, LINE);
    rules[i].addVLineDiff(0, 1, LINE);
    rules[i].addHLineDiff(1, 0, LINE);

    i++;

    /** Rule 13
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addVLinePattern(0, 2, LINE);
    rules[i].addHLinePattern(1, 2, NLINE);

    rules[i].addVLineDiff(1, 1, NLINE);
    rules[i].addHLineDiff(2, 1, NLINE);

    i++;

    /** Rule 14
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addVLinePattern(0, 2, LINE);
    rules[i].addVLinePattern(1, 1, NLINE);
    rules[i].addHLinePattern(2, 1, NLINE);

    rules[i].addHLineDiff(1, 2, NLINE);

    i++;

    /** Rule 15
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addVLinePattern(2, 1, NLINE);
    rules[i].addHLinePattern(2, 0, NLINE);

    rules[i].addVLineDiff(1, 1, NLINE);
    rules[i].addHLineDiff(2, 1, NLINE);

    i++;

    /** Rule 16
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addVLinePattern(0, 1, LINE);
    rules[i].addVLinePattern(0, 2, LINE);
    rules[i].addHLinePattern(1, 2, NLINE);
    rules[i].addHLinePattern(1, 0, NLINE);

    rules[i].addHLineDiff(1, 1, LINE);

    i++;

    /** Rule 17
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, TWO);
    rules[i].addVLinePattern(2, 1, NLINE);
    rules[i].addHLinePattern(2, 0, NLINE);
    rules[i].addHLinePattern(1, 1, NLINE);

    rules[i].addHLineDiff(2, 1, LINE);
    rules[i].addVLineDiff(1, 1, LINE);

    i++;

    /** Rule 18
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, TWO);
    rules[i].addVLinePattern(0, 1, NLINE);
    rules[i].addVLinePattern(0, 2, NLINE);
    rules[i].addHLinePattern(1, 2, NLINE);
    rules[i].addHLinePattern(2, 2, NLINE);

    rules[i].addHLineDiff(1, 0, LINE);
    rules[i].addVLineDiff(2, 2, LINE);

    i++;

    /** Rule 19
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, TWO);
    rules[i].addVLinePattern(0, 1, NLINE);
    rules[i].addVLinePattern(0, 2, NLINE);
    rules[i].addHLinePattern(1, 2, NLINE);

    rules[i].addHLineDiff(1, 0, LINE);

    i++;

    /** Rule 20
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, THREE);
    rules[i].addVLinePattern(0, 2, LINE);

    rules[i].addHLineDiff(2, 1, LINE);
    rules[i].addHLineDiff(1, 2, NLINE);
    rules[i].addVLineDiff(1, 1, LINE);

    i++;

    /** Rule 21
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

    rules[i] = Rule(3, 3);

    rules[i].addNumberPattern(1, 1, THREE);
    rules[i].addVLinePattern(2, 1, NLINE);
    rules[i].addHLinePattern(2, 0, NLINE);

    rules[i].addHLineDiff(2, 1, LINE);
    rules[i].addVLineDiff(1, 1, LINE);

    i++;


    /** Rule 22
     * Before         After
     * .   .   .      .   .   .
     *   3   1
     * .   .   .      . _ .   .
     * x   x
     * .   .   .      .   .   .
     *
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(0, 0, THREE);
    rules[i].addNumberPattern(0, 1, ONE);
    rules[i].addVLinePattern(1, 0, NLINE);
    rules[i].addVLinePattern(1, 1, NLINE);

    rules[i].addHLineDiff(1, 0, LINE);

    i++;

    /** Rule 23
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

    rules[i] = Rule(3, 3);

    rules[i].addVLinePattern(0, 1, NLINE);
    rules[i].addVLinePattern(0, 2, NLINE);
    rules[i].addVLinePattern(2, 2, NLINE);
    rules[i].addHLinePattern(1, 2, LINE);
    rules[i].addHLinePattern(1, 0, NLINE);
    rules[i].addHLinePattern(2, 0, NLINE);
    rules[i].addHLinePattern(2, 2, NLINE);

    rules[i].addVLineDiff(2, 1, LINE);

    i++;

    /** Rule 24
     * Before         After
     *
     * . _ .      .   .
     * |   |
     * .   .      . x .
     *
     */

    rules[i] = Rule(1, 1);

    rules[i].addVLinePattern(0, 0, LINE);
    rules[i].addVLinePattern(0, 1, LINE);
    rules[i].addHLinePattern(0, 0, LINE);

    rules[i].addHLineDiff(1, 0, NLINE);

    i++;

    /** Rule 25
     * Before         After
     *
     * . x .   .     .   .   .
     * x 1
     * .   .   .     .   .   .
     *       1               x
     * .   .   .     .   . x .
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(0, 0, ONE);
    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addVLinePattern(0, 0, NLINE);
    rules[i].addHLinePattern(0, 0, NLINE);

    rules[i].addHLineDiff(2, 1, NLINE);
    rules[i].addVLineDiff(1, 2, NLINE);

    i++;

    /** Rule 26
     * Before         After
     *
     * .   .   .     .   .   .
     *     x
     * . x .   .     .   . x .
     *       1           x
     * .   .   .     .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addVLinePattern(0, 1, NLINE);
    rules[i].addHLinePattern(1, 0, NLINE);


    rules[i].addHLineDiff(1, 1, NLINE);
    rules[i].addVLineDiff(1, 1, NLINE);

    i++;

    /** Rule 27
     * Before         After
     *
     * .   . x .     .   .   .
     *       2               |
     * . - .   .     .   .   .
     *                   x
     * .   .   .     .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(0, 1, TWO);
    rules[i].addHLinePattern(0, 1, NLINE);
    rules[i].addHLinePattern(1, 0, LINE);


    rules[i].addVLineDiff(0, 2, LINE);
    rules[i].addVLineDiff(1, 1, NLINE);

    i++;

    /** Rule 28
     * Before         After
     *
     * .   .   .     .   . - .
     *       2               |
     * . - .   .     .   .   .
     *     |
     * .   .   .     .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(0, 1, TWO);
    rules[i].addHLinePattern(1, 0, LINE);
    rules[i].addVLinePattern(1, 1, LINE);

    rules[i].addHLineDiff(0, 1, LINE);
    rules[i].addVLineDiff(0, 2, LINE);

    i++;

    /** Rule 29
     * Before         After
     *
     * . - .   .     .   .   .
     * | 3
     * .   .   .     .   .   .
     *       1               x
     * .   .   .     .   . x .
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(0, 0, THREE);
    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addHLinePattern(0, 0, LINE);
    rules[i].addVLinePattern(0, 0, LINE);

    rules[i].addHLineDiff(2, 1, NLINE);
    rules[i].addVLineDiff(1, 2, NLINE);

    i++;

    /** Rule 30
     * Before         After
     *
     * .   .   .     . - .   .
     *   3           |
     * .   .   .     .   .   .
     *       1 x
     * .   . x .     .   .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(0, 0, THREE);
    rules[i].addNumberPattern(1, 1, ONE);
    rules[i].addHLinePattern(2, 1, NLINE);
    rules[i].addVLinePattern(1, 2, NLINE);

    rules[i].addHLineDiff(0, 0, LINE);
    rules[i].addVLineDiff(0, 0, LINE);

    i++;

    /**
     * Rule #31
     * Before   After
     * .   .    . x .
     *   0      x   x
     * .   .    . x .
     */

    rules[i] = Rule(1, 1);

    rules[i].addNumberPattern(0, 0, ZERO);

    rules[i].addHLineDiff(0, 0, NLINE);
    rules[i].addHLineDiff(1, 0, NLINE);
    rules[i].addVLineDiff(0, 0, NLINE);
    rules[i].addVLineDiff(0, 1, NLINE);

    i++;

    /**
     * Rule 32
     * Before       After
     * .   .   .    .   . _ .
     *       3              |
     * .   .   .    .   .   .
     *   3          |
     * .   .   .    . _ .   .
     */

    rules[i] = Rule(2, 2);

    rules[i].addNumberPattern(1, 0, THREE);
    rules[i].addNumberPattern(0, 1, THREE);

    rules[i].addHLineDiff(0, 1, LINE);
    rules[i].addHLineDiff(2, 0, LINE);
    rules[i].addVLineDiff(1, 0, LINE);
    rules[i].addVLineDiff(0, 2, LINE);

    i++;

    /**
     * Rule 33
     * Before       After
     * .   .   .    .   .   .
     *                  x
     * .   .   .    .   .   .
     *   3   3      |   |   |
     * .   .   .    .   .   .
     *                  x
     * .   .   .    .   .   .
     */

    rules[i] = Rule(3, 2);

    rules[i].addNumberPattern(1, 0, THREE);
    rules[i].addNumberPattern(1, 1, THREE);

    rules[i].addVLineDiff(1, 0, LINE);
    rules[i].addVLineDiff(1, 1, LINE);
    rules[i].addVLineDiff(1, 2, LINE);
    rules[i].addVLineDiff(0, 1, NLINE);
    rules[i].addVLineDiff(2, 1, NLINE);

    i++;
}
