#include "rule.h"
#include "enums.h"

/* Constructor instantiates new rule based on before and
 * after lattices. */
Rule::Rule(Lattice const & before, Lattice const & after) {
    m_ = before.getHeight();
    n_ = before.getWidth();
    before_ = &before;
    after_ = &after;
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

/* Gives the width of the horizontal line grid based on a given
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
    switch (orient) {
        case UPFLIP:
            i = m_ - i - 1;
        case UP:
            return before_->getNumber(i, j);
        case DOWNFLIP:
            i = m_ - i - 1;
        case DOWN:
            return before_->getNumber(m_-i-1, n_-j-1);
        case LEFTFLIP:
            i = n_ - i - 1;
        case LEFT:
            return before_->getNumber(j, n_-i-1);
        case RIGHTFLIP:
            i = n_ - i - 1;
        case RIGHT:
            return before_->getNumber(m_-j-1, i);
    }
}

/* Translates coordinates for a horizontal line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the before_ lattice. */
Edge Rule::getHLineBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i;
        case UP:
            return before_->getHLine(i, j);
        case DOWNFLIP:
            i = m_ - i;
        case DOWN:
            return before_->getHLine(m_-i, n_-j-1);
        case LEFTFLIP:
            i = n_ - i;
        case LEFT:
            return before_->getVLine(j, n_-i);
        case RIGHTFLIP:
            i = n_ - i;
        case RIGHT:
            return before_->getVLine(m_-j-1, i);
    }
}

/* Translates coordinates for a vertical line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the before_ lattice. */
Edge Rule::getVLineBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i - 1;
        case UP:
            return before_->getVLine(i, j);
        case DOWNFLIP:
            i = m_ - i - 1;
        case DOWN:
            return before_->getVLine(m_-i-1, n_-j);
        case LEFTFLIP:
            i = n_ - i - 1;
        case LEFT:
            return before_->getHLine(j, n_-i-1);
        case RIGHTFLIP:
            i = n_ - i - 1;
        case RIGHT:
            return before_->getHLine(m_-j, i);
    }
}

/* Translates coordinates for a number on a rule in a given
 * orientation to the coordinates for that number in its
 * canonical orientation and returns the value of that number
 * on the after_ lattice. */
Number Rule::getNumberAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i - 1;
        case UP:
            return after_->getNumber(i, j);
        case DOWNFLIP:
            i = m_ - i - 1;
        case DOWN:
            return after_->getNumber(m_-i-1, n_-j-1);
        case LEFTFLIP:
            i = n_ - i - 1;
        case LEFT:
            return after_->getNumber(j, n_-i-1);
        case RIGHTFLIP:
            i = n_ - i - 1;
        case RIGHT:
            return after_->getNumber(m_-j-1, i);
    }
}

/* Translates coordinates for a horizontal line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the after_ lattice. */
Edge Rule::getHLineAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i;
        case UP:
            return after_->getHLine(i, j);
        case DOWNFLIP:
            i = m_ - i;
        case DOWN:
            return after_->getHLine(m_-i, n_-j-1);
        case LEFTFLIP:
            i = n_ - i;
        case LEFT:
            return after_->getVLine(j, n_-i);
        case RIGHTFLIP:
            i = n_ - i;
        case RIGHT:
            return after_->getVLine(m_-j-1, i);
    }
}

/* Translates coordinates for a vertical line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the after_ lattice. */
Edge Rule::getVLineAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i - 1;
        case UP:
            return after_->getVLine(i, j);
        case DOWNFLIP:
            i = m_ - i - 1;
        case DOWN:
            return after_->getVLine(m_-i-1, n_-j);
        case LEFTFLIP:
            i = n_ - i - 1;
        case LEFT:
            return after_->getHLine(j, n_-i-1);
        case RIGHTFLIP:
            i = n_ - i - 1;
        case RIGHT:
            return after_->getHLine(m_-j, i);
    }
}
