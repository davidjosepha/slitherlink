#include "solver.h"
#include "enums.h"
#include "lattice.h"
#include "rule.h"

Solver::Solver(Lattice & grid) {
    grid_ = &grid;
    initRules();
    applyRules();
}

void Solver::applyRules() {
    for  (int x = 0; x < NUM_RULES; x++) {
        for (Orientation orient : (Orientation[]){ UP, DOWN, LEFT, RIGHT }) {
            /* orientation = UP */
            for (int i = 0; i <= grid_->getWidth() - rules_[x].getNumberWidth(orient); i++) {
                for (int j = 0; j <= grid_->getHeight() - rules_[x].getNumberHeight(orient); j++) {
                    if (ruleApplies(i, j, rules_[x], orient)) {
                        applyRule(i, j, rules_[x], orient);
                    }
                }
            }
        }
    }
}

void Solver::applyRule(int i, int j, Rule & rule, Orientation orient) {
    for (int k = 0; k < rule.getNumberHeight(orient); k++) {
        for (int l = 0; l < rule.getNumberWidth(orient); l++) {
            if (rule.getNumberAfter(k, l, orient) != NONE) {
                grid_->setNumber(k + i, l + j, rule.getNumberAfter(k, l, orient));
            }
        }
    }

    for (int k = 0; k < rule.getHLineHeight(orient); k++) {
        for (int l = 0; l < rule.getHLineWidth(orient); l++) {
            if (rule.getHLineAfter(k, l, orient) != EMPTY) {
                grid_->setHLine(k + i, l + j, rule.getHLineAfter(k, l, orient));
            }
        }
    }

    for (int k = 0; k < rule.getVLineHeight(orient); k++) {
        for (int l = 0; l < rule.getVLineWidth(orient); l++) {
            if (rule.getVLineAfter(k, l, orient) != EMPTY) {
                grid_->setVLine(k + i, l + j, rule.getVLineAfter(k, l, orient));
            }
        }
    }
}

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
                rule.getVLineBefore(k, l, orient) != grid_->getVLine(k + rule.getWidth(), l + rule.getHeight())) {
                return false;
            }
        }
    }
    return true;
}


void Solver::initRules() {

    int i = 0;

    /* Rule #1 */
    beforeLattices_[i].initArrays(2, 2);
    beforeLattices_[i].cleanArrays();

    beforeLattices_[i].setNumber(1, 0, THREE);
    beforeLattices_[i].setNumber(0, 1, THREE);

    afterLattices_[i].initArrays(2, 2);
    afterLattices_[i].cleanArrays();

    afterLattices_[i].setHLine(0, 1, LINE);
    afterLattices_[i].setHLine(2, 0, LINE);
    afterLattices_[i].setVLine(1, 0, LINE);
    afterLattices_[i].setVLine(0, 2, LINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;

    /* Rule B */
    beforeLattices_[i].initArrays(3,2);
    beforeLattices_[i].cleanArrays();

    beforeLattices_[i].setNumber(1, 0, THREE);
    beforeLattices_[i].setNumber(1, 1, THREE);

    afterLattices_[i].initArrays(3,2);
    afterLattices_[i].cleanArrays();

    afterLattices_[i].setVLine(1, 0, LINE);
    afterLattices_[i].setVLine(1, 1, LINE);
    afterLattices_[i].setVLine(1, 2, LINE);
    afterLattices_[i].setVLine(0, 1, NLINE);
    afterLattices_[i].setVLine(2, 1, NLINE);

    rules_[i] = Rule(beforeLattices_[i], afterLattices_[i]);
    i++;
}
