#include "contradictions.h"
#include "constants.h"
#include "contradiction.h"

/* Initializes the contradictions array with each contradiction
 * used by the Solver to complete the grid. By convention the
 * contradictions will be represented with width <= height,
 * although each contradiction will be applied in each possible
 * orientation. */
void initContradictions(Contradiction (& contradictions)[NUM_CONTRADICTIONS]) {
    int i = 0;

    /**
     * Contradiction #01
     * .   .   .
     *     x
     * . x . x .
     *     |
     * .   .   .
     */
    contradictions[i] = Contradiction(2, 2);

    contradictions[i].addHLinePattern(1, 0, NLINE);
    contradictions[i].addHLinePattern(1, 1, NLINE);
    contradictions[i].addVLinePattern(0, 1, NLINE);
    contradictions[i].addVLinePattern(1, 1, LINE);

    i++;

    /**
     * Contradiction #02
     * .   .   .
     *     |
     * . _ . _ .
     *
     */
    contradictions[i] = Contradiction(1, 2);

    contradictions[i].addHLinePattern(1, 0, LINE);
    contradictions[i].addHLinePattern(1, 1, LINE);
    contradictions[i].addVLinePattern(0, 1, LINE);

    i++;

    /**
     * Contradiction #03
     * . _ .
     * |   |
     * . _ .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addHLinePattern(0, 0, LINE);
    contradictions[i].addVLinePattern(0, 0, LINE);
    contradictions[i].addHLinePattern(1, 0, LINE);
    contradictions[i].addVLinePattern(0, 1, LINE);

    i++;

    /**
     * Contradiction #04
     * . x .
     * x 3
     * .   .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, THREE);
    contradictions[i].addHLinePattern(0, 0, NLINE);
    contradictions[i].addVLinePattern(0, 0, NLINE);

    i++;

    /**
     * Contradiction #05
     * .   .
     * x 3 x
     * .   .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, THREE);
    contradictions[i].addVLinePattern(0, 0, NLINE);
    contradictions[i].addVLinePattern(0, 1, NLINE);

    i++;

    /**
     * Contradiction #06
     * . _ .
     * | 2
     * . _ .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, TWO);
    contradictions[i].addHLinePattern(0, 0, LINE);
    contradictions[i].addHLinePattern(1, 0, LINE);
    contradictions[i].addVLinePattern(0, 0, LINE);

    i++;

    /**
     * Contradiction #07
     * . x .
     * x 2
     * . x .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, TWO);
    contradictions[i].addHLinePattern(0, 0, NLINE);
    contradictions[i].addHLinePattern(1, 0, NLINE);
    contradictions[i].addVLinePattern(0, 0, NLINE);

    i++;

    /**
     * Contradiction #08
     * . _ .
     * | 1
     * .   .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, ONE);
    contradictions[i].addHLinePattern(0, 0, LINE);
    contradictions[i].addVLinePattern(0, 0, LINE);

    i++;

    /**
     * Contradiction #09
     * .   .
     * | 1 |
     * .   .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, ONE);
    contradictions[i].addVLinePattern(0, 0, LINE);
    contradictions[i].addVLinePattern(0, 1, LINE);

    i++;

    /**
     * Contradiction #10
     * . x .
     * x 1 x
     * . x .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, ONE);
    contradictions[i].addVLinePattern(0, 0, NLINE);
    contradictions[i].addVLinePattern(0, 1, NLINE);
    contradictions[i].addHLinePattern(0, 0, NLINE);
    contradictions[i].addHLinePattern(1, 0, NLINE);

    i++;

    /**
     * Contradiction #11
     * .   .
     * | 0
     * .   .
     */
    contradictions[i] = Contradiction(1, 1);

    contradictions[i].addNumberPattern(0, 0, ZERO);
    contradictions[i].addVLinePattern(0, 0, LINE);

    i++;
}
