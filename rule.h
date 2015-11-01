#ifndef RULE_H
#define RULE_H
#include "enums.h"
#include "lattice.h"

class Rule {
    public:
        Rule() { };
        void initLattices(int m, int n);
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };
        Lattice * getBefore() { return &before_; };
        Lattice * getAfter() { return &after_; };

        int getNumberHeight(Orientation orient) const;
        int getNumberWidth(Orientation orient) const;
        int getHLineHeight(Orientation orient) const;
        int getHLineWidth(Orientation orient) const;
        int getVLineHeight(Orientation orient) const;
        int getVLineWidth(Orientation orient) const;

        Number getNumberBefore(int i, int j, Orientation orient) const;
        Edge getHLineBefore(int i, int j, Orientation orient) const;
        Edge getVLineBefore(int i, int j, Orientation orient) const;

        Number getNumberAfter(int i, int j, Orientation orient) const;
        Edge getHLineAfter(int i, int j, Orientation orient) const;
        Edge getVLineAfter(int i, int j, Orientation orient) const;

    private:
        int m_;
        int n_;
        Lattice before_;
        Lattice after_;
};

#endif
