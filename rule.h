#ifndef RULE_H
#define RULE_H
#include "enums.h"
#include "lattice.h"

class Rule {
    public:
        Rule();
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };
        Lattice const * getBefore() const { return &before_; };
        Lattice const * getAfter() const { return &after_; };

    private:
        int m_;
        int n_;
        Lattice before_;
        Lattice after_;
};

#endif
