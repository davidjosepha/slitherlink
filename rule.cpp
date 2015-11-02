#include "rule.h"
#include <iostream>
#include "enums.h"
#include "rotate.h"

Rule::Rule(int m, int n) {
    m_ = m;
    n_ = n;
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
