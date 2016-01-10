#include "solver.h"
#include <cassert>
#include <vector>
#include "constants.h"
#include "contradiction.h"
#include "enums.h"
#include "epq.h"
#include "grid.h"
#include "rotate.h"
#include "rule.h"
#include "structs.h"

/* Constructor takes a grid as input to solve */
Solver::Solver(Grid & grid, Rule rules[NUM_RULES], Contradiction contradictions[NUM_CONTRADICTIONS], int depth) {
    grid_ = &grid;
    depth_ = depth;

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
        epq_.copySubsetPQ(oldEPQ);
    }

    rules_ = rules;
    contradictions_ = contradictions;

    solve();
}

/* Apply a combination of deterministic rules and
 * recursive guessing to find a solution to a puzzle */
void Solver::solve() {
    while (grid_->getUpdated() && !grid_->isSolved()) {
        applyRules(NUM_RULES);

        for (int d = 0; d < depth_; d++) {
            if (!grid_->getUpdated() && !testContradictions() && !grid_->isSolved()) {
                solveDepth(d);
            }
        }
    }
}

/* Make a guess in each valid position in the graph */
void Solver::solveDepth(int depth) {
    bool usingPrioQueue = true;
    if (usingPrioQueue) {
        int initSize = epq_.size();
        int guesses = 0;

        while (!epq_.empty() && guesses++ < initSize) {
            if (grid_->getUpdated()) {
                applyRules(NUM_RULES - NUM_CONST_RULES);
            }

            PrioEdge pe = epq_.top();

            if (pe.h) {
                makeHLineGuess(pe.coords.i, pe.coords.j, depth);
                if (grid_->getHLine(pe.coords.i, pe.coords.j) == EMPTY) {
                    pe.priority = pe.priority - 1;
                    epq_.push(pe);
                }
            } else {
                makeVLineGuess(pe.coords.i, pe.coords.j, depth);
                if (grid_->getVLine(pe.coords.i, pe.coords.j) == EMPTY) {
                    pe.priority = pe.priority - 1;
                    epq_.push(pe);
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

        /* ensure that if the guess happens to solve the puzzle, we notice */
        if (lineGuess.isSolved()) {
            lineGuess.copy(*grid_);
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

            /* again check if solved */
            if (nLineGuess.isSolved()) {
                nLineGuess.copy(*grid_);
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

        /* ensure that if the guess happens to solve the puzzle, we notice */
        if (lineGuess.isSolved()) {
            lineGuess.copy(*grid_);
            //free(lineGuess.contours_);
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

            /* again check if solved */
            if (nLineGuess.isSolved()) {
                nLineGuess.copy(*grid_);
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

        for (int x = 0; x < numRules; x++) {
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

/* Runs a loop testing each contradiction in each orientation in
 * each valid position on the grid, checking if the contradiction
 * applies, and, if so, returning true. */
bool Solver::testContradictions() const {
    if (grid_->containsClosedContours()) {
        return true;
    }
    for (int x = 0; x < NUM_CONTRADICTIONS; x++) {
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

/* Checks if a contradiction in a given orientation applies to
 * a given region of the grid by checking all non-empty values
 * in the before_ lattice and verifying they correspond to the
 * values in the grid. */
bool Solver::contradictionApplies(int i, int j, Contradiction const & contradiction, Orientation orient) const {
    int m = contradiction.getHeight();
    int n = contradiction.getWidth();

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
