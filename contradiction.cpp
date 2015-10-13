#include "contradiction.h"
#include "enums.h"

/* Constructor instantiates new contradiction based on the given lattice. */
Contradiction::Contradiction(Lattice const & cont) {
    m_ = cont.getHeight();
    n_ = cont.getWidth();
    cont_ = &cont;
}

/* Gives the height of the number grid based on a given
 * orientation. In its upright position, its height is m_.
 * These functions all seem unnecessary for contradictions
 * since I believe they are all square in terms of dimensions. */
int Contradiction::getNumberHeight(Orientation orient) const {
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
int Contradiction::getNumberWidth(Orientation orient) const {
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
int Contradiction::getHLineHeight(Orientation orient) const {
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
int Contradiction::getHLineWidth(Orientation orient) const {

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
int Contradiction::getVLineHeight(Orientation orient) const {

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
int Contradiction::getVLineWidth(Orientation orient) const {
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
Number Contradiction::getNumber(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i - 1;
        case UP:
            return cont_->getNumber(i, j);
        case DOWNFLIP:
            i = m_ - i - 1;
        case DOWN:
            return cont_->getNumber(m_-i-1, n_-j-1);
        case LEFTFLIP:
            i = n_ - i - 1;
        case LEFT:
            return cont_->getNumber(j, n_-i-1);
        case RIGHTFLIP:
            i = n_ - i - 1;
        case RIGHT:
            return cont_->getNumber(m_-j-1, i);
    }
}

/* Translates coordinates for a horizontal line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the before_ lattice. */
Edge Contradiction::getHLine(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i;
        case UP:
            return cont_->getHLine(i, j);
        case DOWNFLIP:
            i = m_ - i;
        case DOWN:
            return cont_->getHLine(m_-i, n_-j-1);
        case LEFTFLIP:
            i = n_ - i;
        case LEFT:
            return cont_->getVLine(j, n_-i);
        case RIGHTFLIP:
            i = n_ - i;
        case RIGHT:
            return cont_->getVLine(m_-j-1, i);
    }
}

/* Translates coordinates for a vertical line on a rule in a
 * given orientation to the coordinates for that line in its
 * canonical orientation and returns the value of that line
 * on the before_ lattice. */
Edge Contradiction::getVLine(int i, int j, Orientation orient) const {
    switch (orient) {
        case UPFLIP:
            i = m_ - i - 1;
        case UP:
            return cont_->getVLine(i, j);
        case DOWNFLIP:
            i = m_ - i - 1;
        case DOWN:
            return cont_->getVLine(m_-i-1, n_-j);
        case LEFTFLIP:
            i = n_ - i - 1;
        case LEFT:
            return cont_->getHLine(j, n_-i-1);
        case RIGHTFLIP:
            i = n_ - i - 1;
        case RIGHT:
            return cont_->getHLine(m_-j, i);
    }
}