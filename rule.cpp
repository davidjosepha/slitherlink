#include "rule.h"
#include "enums.h"

Rule::Rule(Lattice const & before, Lattice const & after) {
    m_ = before.getHeight();
    n_ = before.getWidth();
    before_ = &before;
    after_ = &after;
}

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

Number Rule::getNumberBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        case UP:
            return before_->getNumber(i, j);
        case DOWN:
            return before_->getNumber(m_-i-1, n_-j-1);
        case LEFT:
            return before_->getNumber(j, n_-i-1);
        case RIGHT:
            return before_->getNumber(m_-j-1, i);
        case UPFLIP:
        case DOWNFLIP:
        case LEFTFLIP:
        case RIGHTFLIP:
            return before_->getNumber(i, j);
    }
}

Edge Rule::getHLineBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        case UP:
            return before_->getHLine(i, j);
        case DOWN:
            return before_->getHLine(m_-i, n_-j-1);
        case LEFT:
            //return before_->getHLine(j, n_-i-1);
        case RIGHT:
            //return before_->getHLine(m_-j, i);
        case UPFLIP:
        case DOWNFLIP:
        case LEFTFLIP:
        case RIGHTFLIP:
            return before_->getHLine(i, j);
    }
}

Edge Rule::getVLineBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        case UP:
            return before_->getVLine(i, j);
        case DOWN:
            return before_->getVLine(m_-i-1, n_-j);
        case LEFT:
            //return before_->getVLine(j, n_-i);
        case RIGHT:
            //return before_->getVLine(m_-j-1, i);
        case UPFLIP:
        case DOWNFLIP:
        case LEFTFLIP:
        case RIGHTFLIP:
            return before_->getVLine(i, j);
    }
}

Number Rule::getNumberAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        case UP:
            return after_->getNumber(i, j);
        case DOWN:
            return after_->getNumber(m_-i-1, n_-j-1);
        case LEFT:
            return after_->getNumber(j, n_-i-1);
        case RIGHT:
            return after_->getNumber(m_-j-1, i);
        case UPFLIP:
        case DOWNFLIP:
        case LEFTFLIP:
        case RIGHTFLIP:
            return after_->getNumber(i, j);
    }
}

Edge Rule::getHLineAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        case UP:
            return after_->getHLine(i, j);
        case DOWN:
            return after_->getHLine(m_-i, n_-j-1);
        case LEFT:
            //return after_->getHLine(j, n_-i-1);
        case RIGHT:
            //return after_->getHLine(m_-j, i);
        case UPFLIP:
        case DOWNFLIP:
        case LEFTFLIP:
        case RIGHTFLIP:
            return after_->getHLine(i, j);
    }
}

Edge Rule::getVLineAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        case UP:
            return after_->getVLine(i, j);
        case DOWN:
            return after_->getVLine(m_-i-1, n_-j);
        case LEFT:
            //return after_->getVLine(j, n_-i);
        case RIGHT:
            //return after_->getVLine(m_-j-1, i);
        case UPFLIP:
        case DOWNFLIP:
        case LEFTFLIP:
        case RIGHTFLIP:
            return after_->getVLine(i, j);
    }
}
