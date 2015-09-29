#include "solver.h"
#include "enums.h"
#include "lattice.h"
#include "rule.h"

Solver::Solver(Lattice & grid) {
    grid_ = &grid;
}

void Solver::applyRules() {

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
