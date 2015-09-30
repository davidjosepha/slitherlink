#ifndef RULES_H
#define RULES_H
#include "enums.h"
#include "export.h"
#include "rule.h"

#define NUM_RULES 1

void initRules() {
    Rule rules[NUM_RULES];
    Lattice beforeLattices[NUM_RULES];
    Lattice afterLattices[NUM_RULES];

    int i = 0;

    /* Rule #1 */
    beforeLattices[i].initArrays(2, 2);
    beforeLattices[i].cleanArrays();

    beforeLattices[i].setNumber(1, 0, THREE);
    beforeLattices[i].setNumber(0, 1, THREE);

    afterLattices[i].initArrays(2, 2);
    afterLattices[i].cleanArrays();

    afterLattices[i].setHLine(0, 1, LINE);
    afterLattices[i].setHLine(2, 0, LINE);
    afterLattices[i].setVLine(1, 0, LINE);
    afterLattices[i].setVLine(0, 2, LINE);
    i++;

    /* Rule #2 */
}

#endif
