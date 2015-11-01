#include "rules.h"
#include "constants.h"
#include "rule.h"

/* Initializes the rules array with each deterministic rule
 * used by the Solver to complete the grid. By convention the
 * rules will be represented with width <= height, although
 * each rule will be applied in each possible orientation. */
void initRules(Rule (& rules)[NUM_RULES]) {
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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(3, 3);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(1, 1);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

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

    rules[i] = Rule();
    rules[i].initLattices(2, 2);

    before = rules[i].getBefore();
    after = rules[i].getAfter();

    before->setNumber(0, 0, THREE);
    before->setNumber(1, 1, ONE);
    before->setHLine(2, 1, NLINE);
    before->setVLine(1, 2, NLINE);

    after->setHLine(0, 0, LINE);
    after->setVLine(0, 0, LINE);

    i++;
}
