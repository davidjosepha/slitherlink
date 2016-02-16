#include "solver.h"
#include <cassert>
#include <vector>
#include "contradiction.h"
#include "epq.h"
#include "rotate.h"
#include "rule.h"
#include "../shared/constants.h"
#include "../shared/enums.h"
#include "../shared/grid.h"
#include "../shared/structs.h"

/* Constructor takes a grid as input to solve */
Solver::Solver(Grid & grid, Rule rules[NUM_RULES], Contradiction contradictions[NUM_CONTRADICTIONS], int depth) {
    grid_ = &grid;
    depth_ = depth;

    multipleSolutions_ = false;

    epq_.initEPQ(grid_->getHeight(), grid_->getWidth());

    rules_ = rules;
    contradictions_ = contradictions;

    solve();
}


/* Constructor for when the EPQ should be passed down. */
Solver::Solver(Grid & grid, Rule rules[NUM_RULES], Contradiction contradictions[NUM_CONTRADICTIONS], int depth, EPQ oldEPQ) {
    grid_ = &grid;
    depth_ = depth;

    if (oldEPQ.size() == 0) {
        epq_.initEPQ(grid_->getHeight(), grid_->getWidth());
    } else {
        epq_.copyPQ(oldEPQ);
    }
    multipleSolutions_ = false;

    rules_ = rules;
    contradictions_ = contradictions;

    solve();
}

void Solver::resetSolver() {
    grid_->resetGrid();
    multipleSolutions_ = false;
}

/* Runs a loop testing each contradiction in each orientation in
 * each valid position on the grid, checking if the contradiction
 * applies, and, if so, returning true. */
bool Solver::testContradictions() const {
    if (grid_->containsClosedContours() && !grid_->isSolved()) {
        return true;
    }
    for (int i = 0; i < grid_->getHeight(); i++) {
        for (int j = 0; j < grid_->getWidth(); j++) {
            if (grid_->getContraMatrix(i,j)) {
                for (int x = 0; x < NUM_CONTRADICTIONS; x++) {
                    for (Orientation orient: (Orientation[]){ UP, DOWN, LEFT, RIGHT, UPFLIP, DOWNFLIP, LEFTFLIP, RIGHTFLIP }) {
                        if (contradictionApplies(i, j, contradictions_[x], orient)) {
                            return true;
                        }
                    }
                }
                grid_->setContraMatrix(i, j, false);
            }
        }
    }

    return false;
}

/* Apply a combination of deterministic rules and
 * recursive guessing to find a solution to a puzzle */
void Solver::solve() {
    grid_->setUpdated(true);
    while (grid_->getUpdated() && !grid_->isSolved()) {
        applyRules(NUM_RULES - NUM_CONST_RULES);
        // updatePQ
        //if (epq_.size() < epqSize_/2) {
        //    updateEPQ();
        //}
        for (int d = 0; d < depth_; d++) {
            if (!grid_->getUpdated() && !testContradictions() && !grid_->isSolved() && !multipleSolutions_) {
                solveDepth(d);
            }
        }
    }
}

/* */
void Solver::updateEPQ() {
    epq_.empty();

    int m = grid_->getHeight();
    int n = grid_->getWidth();
    for (int i = 1; i < m ; i++) {
        for (int j = 1; j < n-1; j++) {
            if (grid_->getHLine(i,j) != EMPTY) {
                continue;
            }
            float prio = grid_->getHLine(i,j-1) != EMPTY + grid_->getHLine(i,j+1) != EMPTY +
            grid_->getHLine(i+1,j) != EMPTY + grid_->getHLine(i-1,j) != EMPTY + grid_->getVLine(i-1,j+1) != EMPTY +
            grid_->getVLine(i-1,j) != EMPTY + grid_->getVLine(i,j) != EMPTY + grid_->getVLine(i,j+1) != EMPTY;
            // if (grid_->getHLine(i,j-1) != EMPTY) {
            //     prio = prio + 1;
            //     //do stuff
            // }
            // if (grid_->getHLine(i,j+1) != EMPTY) {
            //     prio = prio + 1;
            //     //do stuff
            // }
            // if (grid_->getHLine(i+1,j) != EMPTY) {
            //     prio = prio + .5;
            //     //do stuff
            // }
            // if (grid_->getHLine(i-1,j) != EMPTY) {
            //     prio = prio + .5;
            //     //do stuff
            // }
            // if (grid_->getVLine(i-1,j+1) != EMPTY) {
            //     prio = prio + 1;
            // }
            // if (grid_->getVLine(i-1,j) != EMPTY) {
            //     prio = prio + 1;
            // }
            // if (grid_->getVLine(i,j) != EMPTY) {
            //     prio = prio + 1;
            // }
            // if (grid_->getVLine(i,j+1) != EMPTY) {
            //     prio = prio + 1;
            // }
            if (prio > 0) {
                epq_.emplace(prio, i, j, true);
            }
        }
    }

    for (int i = 1; i < m-1; i++) {
        for (int j = 1; j < n; j++) {
            if (grid_->getVLine(i,j) != EMPTY) {
                continue;
            }
            float prio = grid_->getVLine(i-1,j) != EMPTY + grid_->getVLine(i+1,j) != EMPTY +
            grid_->getVLine(i,j-1) != EMPTY + grid_->getVLine(i,j+1) != EMPTY + grid_->getHLine(i,j-1) != EMPTY +
            grid_->getHLine(i+1,j-1) != EMPTY + grid_->getHLine(i,j) != EMPTY + grid_->getHLine(i+1,j) != EMPTY;
            // if (grid_->getVLine(i-1,j) != EMPTY) {
            //     prio = prio + 1;
            //     //do stuff
            // }
            // if (grid_->getVLine(i+1,j) != EMPTY) {
            //     prio = prio + 1;
            //     //do stuff
            // }
            // if (grid_->getVLine(i,j-1) != EMPTY) {
            //     prio = prio + .5;
            //     //do stuff
            // }
            // if (grid_->getVLine(i,j+1) != EMPTY) {
            //     prio = prio + .5;
            //     //do stuff
            // }
            // if (grid_->getHLine(i,j-1) != EMPTY) {
            //     prio = prio + 1;
            // }
            // if (grid_->getHLine(i+1,j-1) != EMPTY) {
            //     prio = prio + 1;
            // }
            // if (grid_->getHLine(i,j) != EMPTY) {
            //     prio = prio + 1;
            // }
            // if (grid_->getHLine(i+1,j) != EMPTY) {
            //     prio = prio + 1;
            // }
            if (prio > 0) {
                epq_.emplace(prio, i, j, false);
            }
        }
    }
    epqSize_ = epq_.size();
}


/* Make a guess in each valid position in the graph */
void Solver::solveDepth(int depth) {
    //if (epq_.size() < epqSize_/2) {
    //    updateEPQ();
    //}
    bool usingPrioQueue = true;
    if (usingPrioQueue) {
        int initSize = epq_.size();
        int guesses = 0;

        while (!epq_.empty() && guesses++ < initSize && !multipleSolutions_) {

            PrioEdge pe = epq_.top();

            if (pe.h) {
                makeHLineGuess(pe.coords.i, pe.coords.j, depth);
                if (grid_->getHLine(pe.coords.i, pe.coords.j) == EMPTY) {
                    pe.priority = pe.priority - 1;
                    epq_.push(pe);
                }
                if (grid_->getUpdated()) {
                    break;
                }
            } else {
                makeVLineGuess(pe.coords.i, pe.coords.j, depth);
                if (grid_->getVLine(pe.coords.i, pe.coords.j) == EMPTY) {
                    pe.priority = pe.priority - 1;
                    epq_.push(pe);
                }
                if (grid_->getUpdated()) {
                    break;
                }
            }
            epq_.pop();
        }
    } else {
        for (int i = 0; i < grid_->getHeight()+1; i++) {
            for (int j = 0; j < grid_->getWidth(); j++) {
                applyRules(NUM_RULES - NUM_CONST_RULES);
                makeHLineGuess(i, j, depth);
            }
        }

        for (int i = 0; i < grid_->getHeight(); i++) {
            for (int j = 0; j < grid_->getWidth()+1; j++) {
                applyRules(NUM_RULES - NUM_CONST_RULES);
                makeVLineGuess(i, j, depth);
            }
        }
    }
}

/* Horizontal guess at the given location to the given depth */
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

        /* make a LINE guess */
        lineGuess.setHLine(i, j, LINE);
        Solver lineSolver = Solver(lineGuess, rules_, contradictions_, depth, epq_);

        /* If this guess happens to solve the puzzle we need to make sure that
         * the opposite guess leads to a contradiction, otherwise we know that
         * there might be multiple solutions */
        if (lineGuess.isSolved()) {
            Grid nLineGuess;
            grid_->copy(nLineGuess);
            nLineGuess.setHLine(i, j, NLINE);
            Solver nLineSolver = Solver(nLineGuess, rules_, contradictions_, depth, epq_);
            if (nLineSolver.testContradictions()) {
                /* The opposite guess leads to a contradiction
                 * so the previous found solution is the only one */
                lineGuess.copy(*grid_);
            } else if (nLineGuess.isSolved() || nLineSolver.hasMultipleSolutions()) {
                /* The opposite guess also led to a solution
                 * so there are multiple solutions */
                multipleSolutions_ = true;
            } else {
                /* The opposite guess led to neither a solution or
                 * a contradiction, so we can't say whether there are
                 * more than one solution */
                grid_->setUpdated(false);
            }
            return;
        }
        /* test for contradictions; if we encounter one we set the opposite line */
        else if (lineSolver.testContradictions()) {
            grid_->setHLine(i, j, NLINE);
            return;
        } else {
            Grid nLineGuess;
            grid_->copy(nLineGuess);

            /* make an NLINE guess */
            nLineGuess.setHLine(i, j, NLINE);
            Solver nLineSolver = Solver(nLineGuess, rules_, contradictions_, depth, epq_);

            /* if both guesses led to multiple solutions, we know this puzzle
             * must also lead to another solution */
            if (nLineSolver.hasMultipleSolutions() && lineSolver.hasMultipleSolutions()) {
                multipleSolutions_ = true;
                return;
            }
            /* again check if solved. In this case we already know that we can't
             * get to a solution or contradiction with the opposite guess, so
             * we know we can't conclude whether this is the single solution */
            else if (nLineGuess.isSolved()) {
                grid_->setUpdated(false);
                return;
            }
            /* again check for contradictions */
            else if (nLineSolver.testContradictions()) {
                grid_->setHLine(i, j, LINE);
                return;
            } else {
                grid_->setUpdated(false);

                /* check for things that happen when we make both
                 * guesses; if we find any, we know they must happen */
                intersectGrids(lineGuess, nLineGuess);

                if (grid_->getUpdated()) {
                    return;
                }
            }
        }
    }
}

/* Vertical guess at the given location to the given depth */
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

        /* make a LINE guess */
        lineGuess.setVLine(i, j, LINE);
        Solver lineSolver = Solver(lineGuess, rules_, contradictions_, depth, epq_);

        /* If this guess happens to solve the puzzle we need to make sure that
         * the opposite guess leads to a contradiction, otherwise we know that
         * there might be multiple solutions */
        if (lineGuess.isSolved()) {
            Grid nLineGuess;
            grid_->copy(nLineGuess);
            nLineGuess.setVLine(i, j, NLINE);
            Solver nLineSolver = Solver(nLineGuess, rules_, contradictions_, depth, epq_);
            if (nLineSolver.testContradictions()) {
                /* The opposite guess leads to a contradiction
                 * so the previous found solution is the only one */
                lineGuess.copy(*grid_);
            } else if (nLineGuess.isSolved() || nLineSolver.hasMultipleSolutions()) {
                /* The opposite guess also led to a solution
                 * so there are multiple solutions */
                multipleSolutions_ = true;
            } else {
                /* The opposite guess led to neither a solution or
                 * a contradiction, so we can't say whether there are
                 * more than one solution */
                grid_->setUpdated(false);
            }
            return;
        }
        /* test for contradictions; if we encounter one we set the opposite line */
        else if (lineSolver.testContradictions()) {
            grid_->setVLine(i, j, NLINE);
            return;
        } else {
            Grid nLineGuess;
            grid_->copy(nLineGuess);

            /* make an NLINE guess */
            nLineGuess.setVLine(i, j, NLINE);
            Solver nLineSolver = Solver(nLineGuess, rules_, contradictions_, depth, epq_);

            /* if both guesses led to multiple solutions, we know this puzzle
             * must also lead to another solution */
            if (nLineSolver.hasMultipleSolutions() && lineSolver.hasMultipleSolutions()) {
                multipleSolutions_ = true;
                return;
            }
            /* again check if solved. In this case we already know that we can't
             * get to a solution or contradiction with the opposite guess, so
             * we know we can't conclude whether this is the single solution */
            else if (nLineGuess.isSolved()) {
                grid_->setUpdated(false);
                return;
            }
            /* again check for contradictions */
            else if (nLineSolver.testContradictions()) {
                grid_->setVLine(i, j, LINE);
                return;
            } else {
                grid_->setUpdated(false);

                /* check for things that happen when we make both
                 * guesses; if we find any, we know they must happen */
                intersectGrids(lineGuess, nLineGuess);

                if (grid_->getUpdated()) {
                    return;
                }
            }
        }
    }
}

/* TODO: Comment this function */
bool Solver::spiralNext(int startm, int startn, int & prevm, int & prevn) const {
    int tempm = prevm;
    int tempn = prevn;

    if (prevm + prevn == startm + startn) {
        if (prevn >= startn) {
            prevn++;
        } else {
            prevm--;
        }
    } else if ((startm - prevm) == (startn - prevn)) {
        if (prevm > startm) {
            prevn--;
        } else {
            prevn++;
        }
    } else if (prevm > prevn) {
        if (prevm + prevn > startm + startn) {
            if (prevm > startm) {
                prevn--;
            }
        } else {
            if (prevn < startn) {
                prevm--;
            }
        }
    } else {
        if (prevm + prevn > startm + startn) {
            if (prevn > startn) {
                prevm++;
            }
        } else {
            if (prevm < startm) {
                prevn++;
            }
        }
    }

    return !((tempm == prevm && tempn == prevn) ||
              prevm < 0 ||
              prevn < 0 ||
              prevm >= grid_->getHeight() ||
              prevn >= grid_->getWidth());
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
void Solver::applyRules(int numRules) {
    while (grid_->getUpdated()) {
        grid_->setUpdated(false);

        for (int i = 0; i < grid_->getHeight(); i++) {
            for (int j = 0; j < grid_->getWidth(); j++) {
                if (grid_->getUpdateMatrix(i, j)) {
                    for (int x = 0; x < numRules; x++) {
                        for (Orientation orient : (Orientation[]){ UP, DOWN, LEFT, RIGHT, UPFLIP, DOWNFLIP, LEFTFLIP, RIGHTFLIP }) {
                            if (ruleApplies(i, j, rules_[x], orient)) {
                                applyRule(i, j, rules_[x], orient);
                            }
                        }
                    }
                }
                grid_->setUpdateMatrix(i, j, false);
            }
        }
    }
}

/* Applies a rule in a given orientation to a given region of the
 * grid, overwriting all old values with any applicable values from
 * the after_ lattice for that rule. */
void Solver::applyRule(int i, int j, Rule & rule, Orientation orient) {
    int m = rule.getHeight();
    int n = rule.getWidth();

    std::vector<EdgePosition> const * hLineDiff = rule.getHLineDiff();
    for (int k = 0; k < hLineDiff->size(); k++) {
        EdgePosition pattern = (*hLineDiff)[k];
        Coordinates adjusted = rotateHLine(pattern.coords.i, pattern.coords.j, m, n, orient);

        switch (orient) {
            case UPFLIP:
            case UP:
            case DOWNFLIP:
            case DOWN:
                if (grid_->getHLine(adjusted.i + i, adjusted.j + j) == EMPTY) {
                    grid_->setValid(grid_->setHLine(adjusted.i + i, adjusted.j + j, pattern.edge));
                    grid_->setUpdated(true);
                }
                break;
            case LEFTFLIP:
            case LEFT:
            case RIGHTFLIP:
            case RIGHT:
                if (grid_->getVLine(adjusted.i + i, adjusted.j + j) == EMPTY) {
                    grid_->setValid(grid_->setVLine(adjusted.i + i, adjusted.j + j, pattern.edge));
                    grid_->setUpdated(true);
                }
                break;
        }
    }

    std::vector<EdgePosition> const * vLineDiff = rule.getVLineDiff();
    for (int k = 0; k < vLineDiff->size(); k++) {
        EdgePosition pattern = (*vLineDiff)[k];
        Coordinates adjusted = rotateVLine(pattern.coords.i, pattern.coords.j, m, n, orient);

        switch (orient) {
            case UPFLIP:
            case UP:
            case DOWNFLIP:
            case DOWN:
                if (grid_->getVLine(adjusted.i + i, adjusted.j + j) == EMPTY) {
                    grid_->setValid(grid_->setVLine(adjusted.i + i, adjusted.j + j, pattern.edge));
                    grid_->setUpdated(true);
                }
                break;
            case LEFTFLIP:
            case LEFT:
            case RIGHTFLIP:
            case RIGHT:
                if (grid_->getHLine(adjusted.i + i, adjusted.j + j) == EMPTY) {
                    grid_->setValid(grid_->setHLine(adjusted.i + i, adjusted.j + j, pattern.edge));
                    grid_->setUpdated(true);
                }
                break;
        }
    }
}

/* Checks if a rule in a given orientation applies to a given
 * region of the grid by checking all non-empty values in the
 * before_ lattice and verifying they correspond to the values
 * in the grid. */
bool Solver::ruleApplies(int i, int j, Rule const & rule, Orientation orient) const {
    int m = rule.getHeight();
    int n = rule.getWidth();
    if (i > grid_->getHeight() - rule.getNumberHeight(orient) || j > grid_->getWidth() - rule.getNumberWidth(orient)) {
        return false;
    }

    std::vector<NumberPosition> const * numberPattern = rule.getNumberPattern();
    for (int k = 0; k < numberPattern->size(); k++) {
        NumberPosition pattern = (*numberPattern)[k];
        Coordinates adjusted = rotateNumber(pattern.coords.i, pattern.coords.j, m, n, orient);

        if (pattern.num != grid_->getNumber(adjusted.i + i, adjusted.j + j)) {
            return false;
        }
    }

    std::vector<EdgePosition> const * hLinePattern = rule.getHLinePattern();
    for (int k = 0; k < hLinePattern->size(); k++) {
        EdgePosition pattern = (*hLinePattern)[k];
        Coordinates adjusted = rotateHLine(pattern.coords.i, pattern.coords.j, m, n, orient);

        switch (orient) {
            case UPFLIP:
            case UP:
            case DOWNFLIP:
            case DOWN:
                if (pattern.edge != grid_->getHLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
            case LEFTFLIP:
            case LEFT:
            case RIGHTFLIP:
            case RIGHT:
                if (pattern.edge != grid_->getVLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
        }
    }

    std::vector<EdgePosition> const * vLinePattern = rule.getVLinePattern();
    for (int k = 0; k < vLinePattern->size(); k++) {
        EdgePosition pattern = (*vLinePattern)[k];
        Coordinates adjusted = rotateVLine(pattern.coords.i, pattern.coords.j, m, n, orient);

        switch (orient) {
            case UPFLIP:
            case UP:
            case DOWNFLIP:
            case DOWN:
                if (pattern.edge != grid_->getVLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
            case LEFTFLIP:
            case LEFT:
            case RIGHTFLIP:
            case RIGHT:
                if (pattern.edge != grid_->getHLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
        }
    }

    return true;
}

/* Checks if a contradiction in a given orientation applies to
 * a given region of the grid by checking all non-empty values
 * in the before_ lattice and verifying they correspond to the
 * values in the grid. */
bool Solver::contradictionApplies(int i, int j, Contradiction const & contradiction, Orientation orient) const {
    int m = contradiction.getHeight();
    int n = contradiction.getWidth();

    if (i > grid_->getHeight() - contradiction.getNumberHeight(orient) || j > grid_->getWidth() - contradiction.getNumberWidth(orient)) {
        return false;
    }

    std::vector<NumberPosition> const * numberPattern = contradiction.getNumberPattern();
    for (int k = 0; k < numberPattern->size(); k++) {
        NumberPosition pattern = (*numberPattern)[k];
        Coordinates adjusted = rotateNumber(pattern.coords.i, pattern.coords.j, m, n, orient);

        if (pattern.num != grid_->getNumber(adjusted.i + i, adjusted.j + j)) {
            return false;
        }
    }

    std::vector<EdgePosition> const * hLinePattern = contradiction.getHLinePattern();
    for (int k = 0; k < hLinePattern->size(); k++) {
        EdgePosition pattern = (*hLinePattern)[k];
        Coordinates adjusted = rotateHLine(pattern.coords.i, pattern.coords.j, m, n, orient);

        switch (orient) {
            case UPFLIP:
            case UP:
            case DOWNFLIP:
            case DOWN:
                if (pattern.edge != grid_->getHLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
            case LEFTFLIP:
            case LEFT:
            case RIGHTFLIP:
            case RIGHT:
                if (pattern.edge != grid_->getVLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
        }
    }

    std::vector<EdgePosition> const * vLinePattern = contradiction.getVLinePattern();
    for (int k = 0; k < vLinePattern->size(); k++) {
        EdgePosition pattern = (*vLinePattern)[k];
        Coordinates adjusted = rotateVLine(pattern.coords.i, pattern.coords.j, m, n, orient);

        switch (orient) {
            case UPFLIP:
            case UP:
            case DOWNFLIP:
            case DOWN:
                if (pattern.edge != grid_->getVLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
            case LEFTFLIP:
            case LEFT:
            case RIGHTFLIP:
            case RIGHT:
                if (pattern.edge != grid_->getHLine(adjusted.i + i, adjusted.j + j)) {
                    return false;
                }
                break;
        }
    }

    return true;
}
