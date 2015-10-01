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
        for (int i = 0; i <= grid_->getWidth() - rules_[x].getWidth(); i++) {
            for (int j = 0; j <= grid_->getHeight() - rules_[x].getHeight(); j++) {
                if (ruleApplies(i, j, rules_[x])) {
                    applyRule(i,j,rules_[x]);
                }
            }
        }
    }
}

void Solver::applyRule(int i, int j, Rule & rule) {
    Lattice const * after = rule.getAfter();

    for (int k = 0; k < rule.getHeight(); k++) {
        for (int l = 0; l < rule.getWidth(); l++) {
            if (after->getNumber(k, l) != NONE) {
                grid_->setNumber(k + i, l + j, after->getNumber(k, l));
            }
        }
    }

    for (int k = 0; k < rule.getHeight() + 1; k++) {
        for (int l = 0; l < rule.getWidth(); l++) {
            if (after->getHLine(k, l) != EMPTY) {
                grid_->setHLine(k + i, l + j, after->getHLine(k, l));
            }
        }
    }

    for (int k = 0; k < rule.getHeight(); k++) {
        for (int l = 0; l < rule.getWidth() + 1; l++) {
            if (after->getVLine(k, l) != EMPTY) {
                grid_->setVLine(k + i, l + j, after->getVLine(k, l));
            }
        }
    }
}

bool Solver::ruleApplies(int i, int j, Rule & rule) {
    Lattice const * before = rule.getBefore();

    for (int k = 0; k < rule.getHeight(); k++) {
        for (int l = 0; l < rule.getWidth(); l++) {
            if (before->getNumber(k, l) != NONE and
                before->getNumber(k, l) != grid_->getNumber(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < rule.getHeight() + 1; k++) {
        for (int l = 0; l < rule.getWidth(); l++) {
            if (before->getHLine(k, l) != EMPTY and
                before->getHLine(k, l) != grid_->getHLine(k + i, l + j)) {
                return false;
            }
        }
    }

    for (int k = 0; k < rule.getHeight(); k++) {
        for (int l = 0; l < rule.getWidth() + 1; l++) {
            if (before->getVLine(k, l) != EMPTY and
                before->getVLine(k, l) != grid_->getVLine(k + before->getWidth(), l + before->getHeight())) {
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
}
