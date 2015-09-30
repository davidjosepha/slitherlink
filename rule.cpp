#include "rule.h"
#include "enums.h"

Rule::Rule(Lattice const & before, Lattice const & after) {
    m_ = before.getHeight();
    n_ = before.getWidth();
    before_ = &before;
    after_ = &after;
}
