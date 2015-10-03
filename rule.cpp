#include "rule.h"
#include "enums.h"

Rule::Rule(Lattice const & before, Lattice const & after) {
    m_ = before.getHeight();
    n_ = before.getWidth();
    before_ = &before;
    after_ = &after;
}

Number Rule::getNumberBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        default:
            return before_->getNumber(i, j);
    }
}

Edge Rule::getHLineBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        default:
            return before_->getHLine(i, j);
    }
}

Edge Rule::getVLineBefore(int i, int j, Orientation orient) const {
    switch (orient) {
        default:
            return before_->getVLine(i, j);
    }
}

Number Rule::getNumberAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        default:
            return after_->getNumber(i, j);
    }
}

Edge Rule::getHLineAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        default:
            return after_->getHLine(i, j);
    }
}

Edge Rule::getVLineAfter(int i, int j, Orientation orient) const {
    switch (orient) {
        default:
            return after_->getVLine(i, j);
    }
}
