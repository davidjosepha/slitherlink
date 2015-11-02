#include "rule.h"
#include <iostream>
#include <utility>
#include "enums.h"
#include "rotate.h"

/* Instantiates new rule based on dimensions
 * of before and after lattices, where before
 * reflects the necessary condition for the
 * rule to be applied and after represents
 * only the additional lines and nlines added
 * as a result of the rule. The constructor then
 * instantiates each of the lattices with
 * dimensions m by n. */
void Rule::initLattices(int m, int n) {
    m_ = m;
    n_ = n;

    before_.initArrays(m_, n_);
    after_.initArrays(m_, n_);
}

/* Gives the height of the number grid based on a given
 * orientation. In its upright position, its height is m_. */
int Rule::getNumberHeight(Orientation orient) const {
    switch (orient) {
        case UP:
        case DOWN:
        case UPFLIP:
        case DOWNFLIP:
            return m_;
        case LEFT:
        case RIGHT:
        case LEFTFLIP:
        case RIGHTFLIP:
            return n_;
    }
}

/* Gives the width of the number grid based on a given
 * orientation. In its upright position, its width is n_. */
int Rule::getNumberWidth(Orientation orient) const {
    switch (orient) {
        case UP:
        case DOWN:
        case UPFLIP:
        case DOWNFLIP:
            return n_;
        case LEFT:
        case RIGHT:
        case LEFTFLIP:
        case RIGHTFLIP:
            return m_;
    }
}

/* Gives the height of the horizontal line grid based on a given
 * orientation. In its upright position, its height is n_ + 1. */
int Rule::getHLineHeight(Orientation orient) const {
    switch (orient) {
        case UP:
        case DOWN:
        case UPFLIP:
        case DOWNFLIP:
            return m_ + 1;
        case LEFT:
        case RIGHT:
        case LEFTFLIP:
        case RIGHTFLIP:
            return n_ + 1;
    }
}

/* Gives the width of the horizontal line grid based on a given
 * orientation. In its upright position, its width is n_. */
int Rule::getHLineWidth(Orientation orient) const {
    switch (orient) {
        case UP:
        case DOWN:
        case UPFLIP:
        case DOWNFLIP:
            return n_;
        case LEFT:
        case RIGHT:
        case LEFTFLIP:
        case RIGHTFLIP:
            return m_;
    }
}

/* Gives the height of the vertical line grid based on a given
 * orientation. In its upright position, its height is m_. */
int Rule::getVLineHeight(Orientation orient) const {
    switch (orient) {
        case UP:
        case DOWN:
        case UPFLIP:
        case DOWNFLIP:
            return m_;
        case LEFT:
        case RIGHT:
        case LEFTFLIP:
        case RIGHTFLIP:
            return n_;
    }
}

/* Gives the width of the vertical line grid based on a given
 * orientation. In its upright position, its width is n_ + 1. */
int Rule::getVLineWidth(Orientation orient) const {
    switch (orient) {
        case UP:
        case DOWN:
        case UPFLIP:
        case DOWNFLIP:
            return n_ + 1;
        case LEFT:
        case RIGHT:
        case LEFTFLIP:
        case RIGHTFLIP:
            return m_ + 1;
    }
}

/* Translates coordinates for a number on a rule in a given
 * orientation to the coordinates for that number in its
 * canonical orientation and returns the value of that number
 * on the before_ lattice. */
Number Rule::getNumberBefore(int i, int j, Orientation orient) const {
    std::pair<int, int> adjusted = rotateNumber(i, j, m_, n_, orient);
    return before_.getNumber(adjusted.first, adjusted.second);
}

/* Translates coordinates for a horizontal line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the before_ lattice. */
Edge Rule::getHLineBefore(int i, int j, Orientation orient) const {
    std::pair<int, int> adjusted = rotateHLine(i, j, m_, n_, orient);

    switch (orient) {
        case UPFLIP:
        case UP:
        case DOWNFLIP:
        case DOWN:
            return before_.getHLine(adjusted.first, adjusted.second);
        case LEFTFLIP:
        case LEFT:
        case RIGHTFLIP:
        case RIGHT:
            return before_.getVLine(adjusted.first, adjusted.second);
    }
}

/* Translates coordinates for a vertical line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the before_ lattice. */
Edge Rule::getVLineBefore(int i, int j, Orientation orient) const {
    std::pair<int, int> adjusted = rotateVLine(i, j, m_, n_, orient);

    switch (orient) {
        case UPFLIP:
        case UP:
        case DOWNFLIP:
        case DOWN:
            return before_.getVLine(adjusted.first, adjusted.second);
        case LEFTFLIP:
        case LEFT:
        case RIGHTFLIP:
        case RIGHT:
            return before_.getHLine(adjusted.first, adjusted.second);
    }
}

/* Translates coordinates for a number on a rule in a given
 * orientation to the coordinates for that number in its
 * canonical orientation and returns the value of that number
 * on the after_ lattice. */
Number Rule::getNumberAfter(int i, int j, Orientation orient) const {
    std::pair<int, int> adjusted = rotateNumber(i, j, m_, n_, orient);
    return after_.getNumber(adjusted.first, adjusted.second);
}

/* Translates coordinates for a horizontal line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the after_ lattice. */
Edge Rule::getHLineAfter(int i, int j, Orientation orient) const {
    std::pair<int, int> adjusted = rotateHLine(i, j, m_, n_, orient);

    switch (orient) {
        case UPFLIP:
        case UP:
        case DOWNFLIP:
        case DOWN:
            return after_.getHLine(adjusted.first, adjusted.second);
        case LEFTFLIP:
        case LEFT:
        case RIGHTFLIP:
        case RIGHT:
            return after_.getVLine(adjusted.first, adjusted.second);
    }
}

/* Translates coordinates for a vertical line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the after_ lattice. */
Edge Rule::getVLineAfter(int i, int j, Orientation orient) const {
    std::pair<int, int> adjusted = rotateVLine(i, j, m_, n_, orient);

    switch (orient) {
        case UPFLIP:
        case UP:
        case DOWNFLIP:
        case DOWN:
            return after_.getVLine(adjusted.first, adjusted.second);
        case LEFTFLIP:
        case LEFT:
        case RIGHTFLIP:
        case RIGHT:
            return after_.getHLine(adjusted.first, adjusted.second);
    }
}
