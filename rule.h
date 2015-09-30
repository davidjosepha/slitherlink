#ifndef RULE_H
#define RULE_H
#include "enums.h"
#include "lattice.h"

class Rule {
    public:
        Rule() { };
        Rule(Lattice const & before, Lattice const & after);
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };
        Lattice const * getBefore() const { return before_; };
        Lattice const * getAfter() const { return after_; };

    private:
        int m_;
        int n_;
        Lattice const * before_;
        Lattice const * after_;
};

#endif
